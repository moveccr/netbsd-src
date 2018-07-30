BEGIN {
	print "/* generated by mkdefs.awk */"
	print "#ifndef XPLX_DEFINE"
	print "#define XPLX_DEFINE"
}

# hex to decimal
function h2d(s,   i,c,n,rv)
{
	rv = 0
	for (i = 1; i <= length(s); i++) {
		c = toupper(substr(s, i, 1))
		n = index("0123456789ABCDEF", c)
		if (n == 0) {
			error = 1
			exit error
		}
		rv = rv * 16 + n - 1
	}
	return rv
}

# Global label
$2 ~ /::/ {
	print "// " $0
	sub(/::/, "", $2)
	sub(/:/, "", $1)
	printf("#define %s 0x%s\n", $2, $1);

	k = "GLOBAL_"
	keys[""] = k
	v = h2d($1)
	while (values["", v] != "") v++;
	values["", v] = $2
	counts[""]++
}

$2 ~ /#define/ {
	printf("%s %s %s\n", $2, $3, $4);

	# multiple if statements
	# because match() returns index/length by global variables
	if (match($3, /^XPLX_R_/)) {
		k = substr($3, RSTART, RLENGTH)
	} else
	if (match($3, /^DEVID_/)) {
		k = substr($3, RSTART, RLENGTH)
	} else
	if (match($3, /^[^_]+_(CMD|ENC)_/)) {
		k = substr($3, RSTART, RLENGTH)
	} else {
		next
	}
	sub(k, "", $3)
	keys[k] = k
	values[k, $4] = $3
	counts[k]++
}

END {
	print "#endif /* !XPLX_DEFINE */"
	print ""

	print "#ifdef XPLX_EXTERN_TABLE"
	print "#include \"kv.h\""

	for (k in keys) {
		print "extern const struct kv " keys[k] "[" counts[k] "];"
		print "#define KV_" substr(keys[k], 1, length(keys[k]) - 1) " "\
			keys[k] ", " counts[k]
	}

	print "#endif /* XPLX_EXTERN_TABLE */"
	print ""

	print "#ifdef XPLX_TABLE"
	print "#include \"kv.h\""

	for (k in keys) {
		print "const struct kv " keys[k] "[] = {"
		n = counts[k]
		# order by i
		for (i = 0; n > 0; i++) {
			if (values[k, i] == "") continue
			printf("\t{ %s, \"%s\" },\n",
				k values[k, i],
				values[k, i]);
			n--;
		}
		print "};\n"
	}

	print "#endif /* XPLX_TABLE */"
}
