#Helper file.

#=====================================================================
# Take SAT4J output and the algorithm configuration and write the results into the DB. 
#=====================================================================
def readSAT4JAndWriteToDB(output_file, algorun_config_id, host)
    
    ############### READ FILE.
    content=""
    File.open(output_file, "r"){|output|
	#content = output.gets(nil)
    	while line = output.gets
		content += line unless line =~ /^v/
	end
    }
#    puts output_file
#    puts content
        

    ############### Put the output file into the DB.
#     add_output_cmd = "insert into FH_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
#     output_id = execute_cmd_with_quotes_one_autoincrement(add_output_cmd, false)
      output_id = -1
    
    decisions = -1
    cpu_time = -1
    wallclock_readin = -1
    cpu_readin = -1
    cutoff_time = 0

    ############### Parse output and write runs to DB.
    File.open(output_file, "r"){|output|
	error=false
	while line = output.gets
#		puts line
		if line =~ /c decisions\s*:\s*(\d+)/
			decisions = $1.to_i
		end
		
		if line =~ /c timeout: (#{float_regexp})s/
			cutoff_time = $1.to_f
		end
		
		if line =~ /Wall clock time:\s*(#{float_regexp})/
			wallclock_readin= $1.to_f
		end
		if line =~ /c CPU time \(experimental\)\s*(#{float_regexp})/
			cpu_readin= $1.to_f
		end

		if line =~ /c Total wall clock time \(in seconds\)\s*:\s*(#{float_regexp})/
			wallclock_time = $1.to_f
		end
		if line =~ /c Total CPU time \(experimental, in seconds\)\s*:\s*(#{float_regexp})/
			cpu_time = $1.to_f
		end
		
		if line =~ /s (.*)/
			result = $1
		end
		if line =~ /Problem/
			raise "Problem with parsing!" 
		end
	end	

	res = ""
	if result == "SATISFIABLE"
		res = "SAT"
	elsif result == "UNSATISFIABLE"
		res = "UNSAT"
	elsif result == "UNKNOWN"
		res = "TIMEOUT"
	end

=begin
	if cpu_time < cutoff_time and res == "TIMEOUT" and not error
		puts "================================================================"
		puts "SAT4J was terminated externally before its time was up: #{cpu_time} < #{cutoff_time}"
		$stdout.flush
		
		raise "SAT4J was terminated externally before its time was up: #{output_file}"
	end
=end
	#=== Put the run into the DB.
	require "dbi_ils_accessor.rb"
	if cpu_time < cutoff_time and res == "TIMEOUT"
		writeResultToDB(algorun_config_id, -1, -1, -1, -1, -1, "CRASHED", output_id, host)
#		mysql_cmd = "insert into FH_ALGORUN VALUES(#{}, now(), #{output_id}, '#{host}');"
	else
		writeResultToDB(algorun_config_id, -1, cpu_time, decisions, -1, -1, res, output_id, host)
#		mysql_cmd = "insert into FH_ALGORUN VALUES(#{algorun_config_id}, -1,  #{cpu_time}, #{decisions}, -1, -1, '#{res}', now(), #{output_id}, '#{host}');"
	end
#	puts "Writing to DB with command: #{mysql_cmd}"
#	execute_cmd(mysql_cmd)
#        raise "No SAT4J run output exists in file #{output_file}" unless cpu_time
	puts "Wrote to DB (algorun_config_id=#{algorun_config_id}), finished."
    }
    return true
end
