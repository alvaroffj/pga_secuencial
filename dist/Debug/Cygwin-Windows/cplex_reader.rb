#Helper file.
#==readUbcsatAndWriteToDB takes a UBCSAT run output, the algorithm configuration
#==and the actual command that generated it, and writes the results into the DB. 
def readCPLEXAndWriteToDB(output_file, algorun_config_id, host)
	#===  Setting up variables for run output.
	solved = "CRASHED"
	seed = -1
	best_sol = -1
	best_length = -1
	measured_runlength = -1
	measured_runtime = -1
    
	gap = 1e10
    
	File.open(output_file, "r"){|file|
		while line = file.gets
#			puts "Read line: #{line}"
			
			#########################################################################
			#===  Parsing CPLEX run output
			#########################################################################
			if line =~/(#{float_regexp})%/
				gap = $1.to_f
			end
				
			if line =~ /MIP\s*-\s*Integer optimal solution:\s*Objective\s*=\s*(#{float_regexp})/
				best_sol = $1
				solved = 'SAT'
			end

			if line =~ /MIP\s*-\s*Integer optimal,\s*tolerance\s*\(#{float_regexp}\/#{float_regexp}\):\s*Objective\s*=\s*(#{float_regexp})/
				best_sol = $1
				solved = 'SAT'
			end
			
			
			if line =~ /Solution time\s*=\s*(#{float_regexp})\s*sec\.\s*Iterations\s*=\s*(\d+)\s*Nodes\s*=\s*(\d+)/
				measured_runtime = $1
				iterations = $2
				measured_runlength = $3
			end

			if line =~ /Solution time\s*=\s*(#{float_regexp}) sec\.\s*Iterations =\s*(\d+)/
#			solved = 'SAT'
				measured_runtime = $1
				iterations = $2				
			end

			if line =~ /Solution time =\s*(#{float_regexp}) sec\./
				#solved = 'SAT'
				measured_runtime = $1
				iterations = 0
			end
			
			if line =~ /Optimal:\s*Objective =\s*#{float_regexp}/
				solved = 'SAT'
			end

			if line =~ /Infeasible/
				solved = 'UNSAT'
			end
			
			if line =~ /MIP\s*-\s*Time limit exceeded, integer feasible:\s*Objective\s*=\s*(#{float_regexp})/
				best_sol = $1
				solved = 'TIMEOUT'
			end
			
			if line =~ /MIP - Time limit exceeded, no integer solution./
				solved = 'TIMEOUT'
			end
			
			if line =~ /CPLEX Error  1001: Out of memory./
				solved = 'TIMEOUT'
			end
			
			if line =~ /CPLEX Error  3019: Failure to solve MIP subproblem./
				solved = 'TIMEOUT'
			end

			if line =~ /CPLEX Error/
				solved = 'TIMEOUT'
			end
			
			if line =~ /Time limit exceeded/
				solved = 'TIMEOUT'
			end
			
#			if line =~ /Filesize limit exceeded/
#				solved = 'TIMEOUT'
#			end

			if line =~ /Solution time =\s*(#{float_regexp})\s*sec\.\s*Iterations\s*=\s*(\d+)\s*Nodes\s*=\s*\((\d+)\)\s*\((\d+)\)/
				measured_runtime = $1
				iterations = $2
				measured_runlength = $3
			end
			
			return false if line =~ /Failed to initialize CPLEX environment./
		end
		best_sol = gap # This is really what we want to minimize.

#		raise "Error: solved neither TIMEOUT nor SAT - probably parsing problem. Here's the complete output: #{content}"

		if solved == "CRASHED"
			puts "\n\n==============================================\n\nWARNING: CPLEX crashed -> most likely file not found or no license\n\n=======================\n"
			raise "No such file or directory: CPLEX crashed -> most likely file not found or no license\n\n======================="
#			Incomplete CPLEX run in file #{output_file} with algorun_confid_id #{algorun_config_id} on host #{host}. See output id #{output_id} or complete output: #{content}"
		else

			############### READ FILE.
			content=""
			File.open(output_file, "r"){|file|
				content = file.gets(nil)
			}
			content = content.slice(0,1000000) if content.length > 1000000

			############### Put the output file into the DB.
#			add_output_cmd = "insert into FH_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
#			output_id = execute_cmd_with_quotes_one_autoincrement(add_output_cmd, false)
			output_id = -1
			
                        require "dbi_ils_accessor.rb"
			writeResultToDB(algorun_config_id, seed, measured_runtime, measured_runlength, best_sol, best_length, solved, output_id, host)
		end
	}
	

	return true
end
