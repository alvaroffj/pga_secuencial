require "param_reader.rb"
if ARGV.length < 3
	puts "CPLEX wrapper: You specify an instance file, a logfilename, a timeout and CPLEX parameters, and I call CPLEX with them and save the results in the specified logfile. Then I read that file and output its result for ParamILS."
	puts "USAGE: ruby cplex_wrapper.rb <instance_file> <logfile> <timeout> [-<param1> <value param1>] [-<param2> <value param2>] ..."
	exit
end

instance_relname = ARGV[0]
cutofftime = ARGV[1]
outfile = ARGV[ARGV.length-1] # easier to pass this in the last argument.

cutofflength = 2100000000

param_lines = []
2.step(ARGV.length-2, 2){|i|
	param = ARGV[i].sub(/^-/,"")
	set_cmd = param.gsub(/_/," ")
	param_lines << "set #{set_cmd} #{ARGV[i+1]}"
}

#=== This is a double File.popen construct: I'm calling ruby on the command line to call CPLEX and output something. That something is read in with File.popen again.
cmd = "ruby -e 'File.popen(\"use cplex; /cs/local/lib/pkg/cplex-10.1.1/bin/cplex\",\"w\"){|file| "

cplex_lines = []
cplex_lines << "read #{instance_relname}"
#cplex_lines << "set clocktype 2"
#cplex_lines << "set logfile #{outfile}"
cplex_lines << "set mip limits nodes #{cutofflength}"
cplex_lines << "set timelimit #{cutofftime}"

#=== Set parameters.
cplex_lines += param_lines

cplex_lines << "display settings all"

cplex_lines << "opt"
cplex_lines << "quit"

cplex_lines.map{|line| cmd += "file.puts \"#{line}\"; "}
cmd += "}'"

puts "Calling: #{cmd} > #{outfile}"

system("#{cmd} > #{outfile}")

=begin
inner_exit = $?
puts "inner exit: #{inner_exit}"
puts "Outfile: #{outfile}"
=end

=begin
File.open(outfile, "w"){|out|
#	puts "Calling cmd: #{cmd}"
	File.popen(cmd){|file|
		while line = file.gets
#			puts line
			out.puts line
		end
	}
}
=end