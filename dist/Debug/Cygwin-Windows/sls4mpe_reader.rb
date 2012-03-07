#Helper file.
require "dbi_ils_accessor.rb"

#==readSLS4MPEAndWriteToDB takes a SLS4MPE run output, the algorithm configuration
#==and the actual command that generated it, and writes the results into the DB. 
def readSLS4MPEAndWriteToDB(sls4mpe_output_file, algorun_config_id, host)
    
    ############### READ FILE.
    content=""
    File.open(sls4mpe_output_file, "r"){|sls4mpe|
    	content = sls4mpe.gets(nil)
    }
#    puts sls4mpe_output_file
#    puts content
        

    ############### Put the output file into the DB.
#    add_sls4mpe_output_cmd = "insert into ALGO_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
    add_sls4mpe_output_cmd = "insert into FH_OUTPUT (OUTPUT_TEXT) values (\"#{content}\")"
    sls4mpe_output_id = execute_cmd_with_quotes_one_autoincrement(add_sls4mpe_output_cmd, false)
    
    
    ############### Parse output and write runs to DB.
    File.open(sls4mpe_output_file, "r"){|sls4mpe|
        mysql_cmds = []
       	run=1;
    	while line = sls4mpe.gets
        	if line =~ /begin solution \d+/
            	#=== Read solution output from SLS4MPE. (run number not recorded)
			line = sls4mpe.gets
			raise "" unless line =~ /best (#{float_regexp})/
			best_sol = $1.to_f
			
			line = sls4mpe.gets
			raise "" unless line =~ /time (#{float_regexp})/
			measured_runtime = $1.to_f
			
			line = sls4mpe.gets
			raise "" unless line =~ /iteration (\d+)/
			iteration = $1.to_i
			
			line = sls4mpe.gets
			raise "" unless line =~ /flip (\d+)/
			measured_runlength = $1.to_i
			
			line = sls4mpe.gets
			raise "" unless line =~ /seed (\d+)/
			seed = $1.to_i

			line = sls4mpe.gets
			line = sls4mpe.gets
			raise "" unless line =~ /Optimal MPE found:/
			if line =~ /yes/
				found = 1
			else
				found = 0
			end
			
#        		best_length = $4.to_i -- have not recorded step of best.
        
        		#=== Put the run into the DB.
#        		puts line
			writeResultToDB(algorun_config_id, seed, measured_runtime, measured_runlength, best_sol, "NULL", "#{found ==1 ? 'SAT' : 'TIMEOUT'}", sls4mpe_output_id, host)

#        		mysql_cmd = "insert into FH_ALGORUN VALUES(#{algorun_config_id}, #{seed},  #{measured_runtime}, #{measured_runlength}, #{best_sol}, NULL, '#{found ==1 ? 'SAT' : 'TIMEOUT'}', now(), #{sls4mpe_output_id}, '#{host}');"
#        		puts "MySQLCommand: #{mysql_cmd}"
#			mysql_cmds << mysql_cmd
        #				execute_cmd(mysql_cmd, false)
        
#			if run.modulo(100)==0
#			     execute_cmd(mysql_cmds.join("\n"), false)
#			     execute_cmds(mysql_cmds)
#			     mysql_cmds = []
#			end
		        run = run+1
        	end
        end
#    	puts mysql_cmds
        raise "No such file or directory: No SLS4MPE run output exists in file #{sls4mpe_output_file} - see output id #{sls4mpe_output_id}" unless run > 1
#        execute_cmd(mysql_cmds.join("\n"), false)
#     execute_cmds(mysql_cmds)
    }
    return true
end