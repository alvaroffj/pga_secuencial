#Helper file.

#=====================================================================
# Take SAT4J output and the algorithm configuration and write the results into the DB. 
#=====================================================================
def readSpearAndWriteToDB(output_file, algorun_config_id, seed, host)
    ############### READ FILE.
    content=""
    File.open(output_file, "r"){|output|
    	content = output.gets(nil)
    }
#    puts output_file
#    puts content
        
    ############### Put the output file into the DB.
#    add_output_cmd = "insert into FH_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
#    output_id = execute_cmd_with_quotes_one_autoincrement(add_output_cmd, false)
output_id = -1

    ############### Parse output and write runs to DB.
    File.open(output_file, "r"){|output|
		res = "CRASHED"
		decisions = -1
		cpu_time = -1
		
		while line = output.gets
#		puts line
		if line =~ /c Decisions:\s*(\d+)/
			decisions = $1.to_i
		end
		
		if line =~ /c Total runtime:\s*(#{float_regexp})/
			cpu_time= $1.to_f
		end

		res = "SAT" if line =~ /s SATISFIABLE/
		res = "UNSAT" if line =~ /s UNSATISFIABLE/
		res = "TIMEOUT" if line =~ /s UNKNOWN/
	end	
        raise "No such file or directory: No Spear run output exists in file #{output_file}" unless res

	#=== Put the run into the DB.
#	mysql_cmd = "insert into FH_ALGORUN VALUES(#{algorun_config_id}, #{seed},  #{cpu_time}, #{decisions}, -1, -1, '#{res}', now(), #{output_id}, '#{host}');"
	raise "No such file or directory: Spear CRASHED for algorun_config_id #{algorun_config_id} on host #{host}, see output id #{output_id} or complete output: #{content}" if res=="CRASHED"
	require "dbi_ils_accessor.rb"
	writeResultToDB(algorun_config_id, seed, cpu_time, decisions, -1, -1, res, output_id, host)

#	puts "Writing to DB with command: #{mysql_cmd}"
#	execute_cmd(mysql_cmd)
	puts "Wrote to DB (algorun_config_id=#{algorun_config_id}), finished."
    }
    return true
end
