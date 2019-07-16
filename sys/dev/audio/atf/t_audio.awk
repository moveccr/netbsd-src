BEGIN {
	n = 0
	print "#"
	print "# This file is re-generated from t_audio.awk"
	print "#"
	print ""
	print "# Call real test program, and then dispatch the result for atf."
	print "h_audio() {"
	print "	local testname=$1"
	print "	outmsg=`$(atf_get_srcdir)/h_audio -AR $testname`"
	print "	local retval=$?"
	print "	if [ \"$retval\" = \"0\" ]; then"
	print "		atf_pass"
	print "	elif [ \"$retval\" = \"1\" ]; then"
	print "		atf_fail \"$outmsg\""
	print "	elif [ \"$retval\" = \"2\" ]; then"
	print "		atf_skip \"$outmsg\""
	print "	else"
	print "		atf_fail \"unknown error $retval\""
	print "	fi"
	print "}"
	print ""
}
match($0, /^DEF\([^\)]*\)/) {
	name = substr($0, RSTART + 4, RLENGTH - 5)
	tests[n] = name
	n++;
	
	print "atf_test_case " name
	print name "_head() {"
	print "}"
	print name "_body() {"
	print "	h_audio " name
	print "}"
	print ""
}
END {
	print "atf_init_test_cases() {"
	for (i = 0; i < length(tests); i++) {
		print "\tatf_add_test_case " tests[i]
	}
	print "}"
}
