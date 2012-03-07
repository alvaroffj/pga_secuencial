#Helper file.
#==readUbcsatAndWriteToDB takes a UBCSAT run output, the algorithm configuration
#==and the actual command that generated it, and writes the results into the DB. 
def readUbcsatAndWriteToDB(ubcsat_output_file, algorun_config_id, host)
    
    ############### READ FILE.
    content=""
    File.open(ubcsat_output_file, "r"){|ubcsat|
    	content = ubcsat.gets(nil)
    }
#    puts ubcsat_output_file
#    puts content
        

    ############### Put the output file into the DB.
#    add_ubcsat_output_cmd = "insert into FH_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
#    ubcsat_output_id = execute_cmd_with_quotes_one_autoincrement(add_ubcsat_output_cmd, false)
	ubcsat_output_id = -1
    
    
    ############### Parse output and write runs to DB.
    File.open(ubcsat_output_file, "r"){|ubcsat|
        mysql_cmds = []
       	run=1;
    	while line = ubcsat.gets
        	next if line =~ /#/
        	if line =~ /\d+\s+(\d+)\s+(\d+)\s+(\d)+\s+(\d+)\s+(\d+)\s+(#{float_regexp})/
            	#=== Read run output from UBCSAT. (run number not recorded)
            	found = $1.to_i
        		seed = $2.to_i
        		best_sol = $3.to_i
        		best_length = $4.to_i
        		measured_runlength = $5.to_i
        		measured_runtime = $6.to_f
        
        		#=== Put the run into the DB.
#        		puts line
                        require "dbi_ils_accessor.rb"
			writeResultToDB(algorun_config_id, seed, measured_runtime, measured_runlength, best_sol, best_length, "#{found ==1 ? 'SAT' : 'TIMEOUT'}", ubcsat_output_id, host)

#        		mysql_cmd = "insert into FH_ALGORUN VALUES(#{algorun_config_id}, #{seed},  #{measured_runtime}, #{measured_runlength}, #{best_sol}, #{best_length}, '#{found ==1 ? 'SAT' : 'TIMEOUT'}', now(), #{ubcsat_output_id}, '#{host}');"
#        		puts "MySQLCommand: #{mysql_cmd}"
#			mysql_cmds << mysql_cmd
        #				execute_cmd(mysql_cmd, false)
        
#			if run.modulo(100)==0
#			     execute_cmds(mysql_cmds)
#			     execute_cmd(mysql_cmds.join("\n"), false)
#			     mysql_cmds = []
#			end
		        run = run+1
        	end
        end
#    	puts mysql_cmds
        raise "No such file or directory : No UBCSAT run output exists in file #{ubcsat_output_file} - see output id #{ubcsat_output_id}" unless run > 1
#        execute_cmd(mysql_cmds.join("\n"), false)
#        execute_cmds(mysql_cmds)
    }
    return true
end
