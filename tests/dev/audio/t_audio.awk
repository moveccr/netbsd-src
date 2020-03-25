#	$NetBSD: t_audio.awk,v 1.1 2020/02/11 07:03:16 isaki Exp $
#
# Copyright (C) 2019 Tetsuya Isaki. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#

BEGIN {
	n = 0
	print "#"
	print "# This file is regenerated by t_audio.awk"
	print "#"
	print ""
	print "# Call real test program, and then dispatch the result for atf."
	print "h_audio() {"
	print "	local testname=$1"
	print "	local outfile=/tmp/t_audio_$testname.$$"
	print "	$(atf_get_srcdir)/audiotest -ARe $testname > $outfile"
	print "	local retval=$?"
	print "	# Discard rump outputs..."
	print "	outmsg=`cat $outfile | grep -v '^\\['`"
	print "	rm -f $outfile"
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
# Gather only tab-indented ENT().  I.e.,
# ^\tENT(testname)     ... Enable in audiotest and add it to atf
# ^//\tENT(testname)   ... Disable in audiotest and also don't add to atf
# ^/**/\tENT(testname) ... Enable in audiotest but don't add to atf
match($0, /^\tENT\([^\)]*\)/) {
	name = substr($0, RSTART + 5, RLENGTH - 6)
	tests[n] = name
	n++;

	print "atf_test_case " name
	print name "_head() { }"
	print name "_body() {"
	print "\th_audio " name
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
