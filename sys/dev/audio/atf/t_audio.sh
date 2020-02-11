#
# This file is regenerated by t_audio.awk
#

# Call real test program, and then dispatch the result for atf.
h_audio() {
	local testname=$1
	local outfile=/tmp/t_audio_$testname.$$
	$(atf_get_srcdir)/audiotest -AR $testname > $outfile
	local retval=$?
	# Discard rump outputs...
	outmsg=`cat $outfile | grep -v '^\['`
	rm -f $outfile
	if [ "$retval" = "0" ]; then
		atf_pass
	elif [ "$retval" = "1" ]; then
		atf_fail "$outmsg"
	elif [ "$retval" = "2" ]; then
		atf_skip "$outmsg"
	else
		atf_fail "unknown error $retval"
	fi
}

atf_test_case open_mode_RDONLY
open_mode_RDONLY_head() { }
open_mode_RDONLY_body() {
	h_audio open_mode_RDONLY
}

atf_test_case open_mode_WRONLY
open_mode_WRONLY_head() { }
open_mode_WRONLY_body() {
	h_audio open_mode_WRONLY
}

atf_test_case open_mode_RDWR
open_mode_RDWR_head() { }
open_mode_RDWR_body() {
	h_audio open_mode_RDWR
}

atf_test_case open_audio_RDONLY
open_audio_RDONLY_head() { }
open_audio_RDONLY_body() {
	h_audio open_audio_RDONLY
}

atf_test_case open_audio_WRONLY
open_audio_WRONLY_head() { }
open_audio_WRONLY_body() {
	h_audio open_audio_WRONLY
}

atf_test_case open_audio_RDWR
open_audio_RDWR_head() { }
open_audio_RDWR_body() {
	h_audio open_audio_RDWR
}

atf_test_case open_sound_RDONLY
open_sound_RDONLY_head() { }
open_sound_RDONLY_body() {
	h_audio open_sound_RDONLY
}

atf_test_case open_sound_WRONLY
open_sound_WRONLY_head() { }
open_sound_WRONLY_body() {
	h_audio open_sound_WRONLY
}

atf_test_case open_sound_RDWR
open_sound_RDWR_head() { }
open_sound_RDWR_body() {
	h_audio open_sound_RDWR
}

atf_test_case open_audioctl_RDONLY
open_audioctl_RDONLY_head() { }
open_audioctl_RDONLY_body() {
	h_audio open_audioctl_RDONLY
}

atf_test_case open_audioctl_WRONLY
open_audioctl_WRONLY_head() { }
open_audioctl_WRONLY_body() {
	h_audio open_audioctl_WRONLY
}

atf_test_case open_audioctl_RDWR
open_audioctl_RDWR_head() { }
open_audioctl_RDWR_body() {
	h_audio open_audioctl_RDWR
}

atf_test_case open_sound_sticky
open_sound_sticky_head() { }
open_sound_sticky_body() {
	h_audio open_sound_sticky
}

atf_test_case open_audioctl_sticky
open_audioctl_sticky_head() { }
open_audioctl_sticky_body() {
	h_audio open_audioctl_sticky
}

atf_test_case open_simul_RDONLY_RDONLY
open_simul_RDONLY_RDONLY_head() { }
open_simul_RDONLY_RDONLY_body() {
	h_audio open_simul_RDONLY_RDONLY
}

atf_test_case open_simul_RDONLY_WRONLY
open_simul_RDONLY_WRONLY_head() { }
open_simul_RDONLY_WRONLY_body() {
	h_audio open_simul_RDONLY_WRONLY
}

atf_test_case open_simul_RDONLY_RDWR
open_simul_RDONLY_RDWR_head() { }
open_simul_RDONLY_RDWR_body() {
	h_audio open_simul_RDONLY_RDWR
}

atf_test_case open_simul_WRONLY_RDONLY
open_simul_WRONLY_RDONLY_head() { }
open_simul_WRONLY_RDONLY_body() {
	h_audio open_simul_WRONLY_RDONLY
}

atf_test_case open_simul_WRONLY_WRONLY
open_simul_WRONLY_WRONLY_head() { }
open_simul_WRONLY_WRONLY_body() {
	h_audio open_simul_WRONLY_WRONLY
}

atf_test_case open_simul_WRONLY_RDWR
open_simul_WRONLY_RDWR_head() { }
open_simul_WRONLY_RDWR_body() {
	h_audio open_simul_WRONLY_RDWR
}

atf_test_case open_simul_RDWR_RDONLY
open_simul_RDWR_RDONLY_head() { }
open_simul_RDWR_RDONLY_body() {
	h_audio open_simul_RDWR_RDONLY
}

atf_test_case open_simul_RDWR_WRONLY
open_simul_RDWR_WRONLY_head() { }
open_simul_RDWR_WRONLY_body() {
	h_audio open_simul_RDWR_WRONLY
}

atf_test_case open_simul_RDWR_RDWR
open_simul_RDWR_RDWR_head() { }
open_simul_RDWR_RDWR_body() {
	h_audio open_simul_RDWR_RDWR
}

atf_test_case open_multiuser_0
open_multiuser_0_head() { }
open_multiuser_0_body() {
	h_audio open_multiuser_0
}

atf_test_case open_multiuser_1
open_multiuser_1_head() { }
open_multiuser_1_body() {
	h_audio open_multiuser_1
}

atf_test_case write_PLAY_ALL
write_PLAY_ALL_head() { }
write_PLAY_ALL_body() {
	h_audio write_PLAY_ALL
}

atf_test_case write_PLAY
write_PLAY_head() { }
write_PLAY_body() {
	h_audio write_PLAY
}

atf_test_case read
read_head() { }
read_body() {
	h_audio read
}

atf_test_case rept_write
rept_write_head() { }
rept_write_body() {
	h_audio rept_write
}

atf_test_case rept_read
rept_read_head() { }
rept_read_body() {
	h_audio rept_read
}

atf_test_case rdwr_fallback_RDONLY
rdwr_fallback_RDONLY_head() { }
rdwr_fallback_RDONLY_body() {
	h_audio rdwr_fallback_RDONLY
}

atf_test_case rdwr_fallback_WRONLY
rdwr_fallback_WRONLY_head() { }
rdwr_fallback_WRONLY_body() {
	h_audio rdwr_fallback_WRONLY
}

atf_test_case rdwr_fallback_RDWR
rdwr_fallback_RDWR_head() { }
rdwr_fallback_RDWR_body() {
	h_audio rdwr_fallback_RDWR
}

atf_test_case rdwr_two_RDONLY_RDONLY
rdwr_two_RDONLY_RDONLY_head() { }
rdwr_two_RDONLY_RDONLY_body() {
	h_audio rdwr_two_RDONLY_RDONLY
}

atf_test_case rdwr_two_RDONLY_WRONLY
rdwr_two_RDONLY_WRONLY_head() { }
rdwr_two_RDONLY_WRONLY_body() {
	h_audio rdwr_two_RDONLY_WRONLY
}

atf_test_case rdwr_two_RDONLY_RDWR
rdwr_two_RDONLY_RDWR_head() { }
rdwr_two_RDONLY_RDWR_body() {
	h_audio rdwr_two_RDONLY_RDWR
}

atf_test_case rdwr_two_WRONLY_RDONLY
rdwr_two_WRONLY_RDONLY_head() { }
rdwr_two_WRONLY_RDONLY_body() {
	h_audio rdwr_two_WRONLY_RDONLY
}

atf_test_case rdwr_two_WRONLY_WRONLY
rdwr_two_WRONLY_WRONLY_head() { }
rdwr_two_WRONLY_WRONLY_body() {
	h_audio rdwr_two_WRONLY_WRONLY
}

atf_test_case rdwr_two_WRONLY_RDWR
rdwr_two_WRONLY_RDWR_head() { }
rdwr_two_WRONLY_RDWR_body() {
	h_audio rdwr_two_WRONLY_RDWR
}

atf_test_case rdwr_two_RDWR_RDONLY
rdwr_two_RDWR_RDONLY_head() { }
rdwr_two_RDWR_RDONLY_body() {
	h_audio rdwr_two_RDWR_RDONLY
}

atf_test_case rdwr_two_RDWR_WRONLY
rdwr_two_RDWR_WRONLY_head() { }
rdwr_two_RDWR_WRONLY_body() {
	h_audio rdwr_two_RDWR_WRONLY
}

atf_test_case rdwr_two_RDWR_RDWR
rdwr_two_RDWR_RDWR_head() { }
rdwr_two_RDWR_RDWR_body() {
	h_audio rdwr_two_RDWR_RDWR
}

atf_test_case rdwr_simul
rdwr_simul_head() { }
rdwr_simul_body() {
	h_audio rdwr_simul
}

atf_test_case drain_incomplete
drain_incomplete_head() { }
drain_incomplete_body() {
	h_audio drain_incomplete
}

atf_test_case drain_pause
drain_pause_head() { }
drain_pause_body() {
	h_audio drain_pause
}

atf_test_case drain_onrec
drain_onrec_head() { }
drain_onrec_body() {
	h_audio drain_onrec
}

atf_test_case mmap_mode_RDONLY_NONE
mmap_mode_RDONLY_NONE_head() { }
mmap_mode_RDONLY_NONE_body() {
	h_audio mmap_mode_RDONLY_NONE
}

atf_test_case mmap_mode_RDONLY_READ
mmap_mode_RDONLY_READ_head() { }
mmap_mode_RDONLY_READ_body() {
	h_audio mmap_mode_RDONLY_READ
}

atf_test_case mmap_mode_RDONLY_WRITE
mmap_mode_RDONLY_WRITE_head() { }
mmap_mode_RDONLY_WRITE_body() {
	h_audio mmap_mode_RDONLY_WRITE
}

atf_test_case mmap_mode_RDONLY_READWRITE
mmap_mode_RDONLY_READWRITE_head() { }
mmap_mode_RDONLY_READWRITE_body() {
	h_audio mmap_mode_RDONLY_READWRITE
}

atf_test_case mmap_mode_WRONLY_NONE
mmap_mode_WRONLY_NONE_head() { }
mmap_mode_WRONLY_NONE_body() {
	h_audio mmap_mode_WRONLY_NONE
}

atf_test_case mmap_mode_WRONLY_READ
mmap_mode_WRONLY_READ_head() { }
mmap_mode_WRONLY_READ_body() {
	h_audio mmap_mode_WRONLY_READ
}

atf_test_case mmap_mode_WRONLY_WRITE
mmap_mode_WRONLY_WRITE_head() { }
mmap_mode_WRONLY_WRITE_body() {
	h_audio mmap_mode_WRONLY_WRITE
}

atf_test_case mmap_mode_WRONLY_READWRITE
mmap_mode_WRONLY_READWRITE_head() { }
mmap_mode_WRONLY_READWRITE_body() {
	h_audio mmap_mode_WRONLY_READWRITE
}

atf_test_case mmap_mode_RDWR_NONE
mmap_mode_RDWR_NONE_head() { }
mmap_mode_RDWR_NONE_body() {
	h_audio mmap_mode_RDWR_NONE
}

atf_test_case mmap_mode_RDWR_READ
mmap_mode_RDWR_READ_head() { }
mmap_mode_RDWR_READ_body() {
	h_audio mmap_mode_RDWR_READ
}

atf_test_case mmap_mode_RDWR_WRITE
mmap_mode_RDWR_WRITE_head() { }
mmap_mode_RDWR_WRITE_body() {
	h_audio mmap_mode_RDWR_WRITE
}

atf_test_case mmap_mode_RDWR_READWRITE
mmap_mode_RDWR_READWRITE_head() { }
mmap_mode_RDWR_READWRITE_body() {
	h_audio mmap_mode_RDWR_READWRITE
}

atf_test_case mmap_len
mmap_len_head() { }
mmap_len_body() {
	h_audio mmap_len
}

atf_test_case mmap_twice
mmap_twice_head() { }
mmap_twice_body() {
	h_audio mmap_twice
}

atf_test_case mmap_multi
mmap_multi_head() { }
mmap_multi_body() {
	h_audio mmap_multi
}

atf_test_case poll_mode_RDONLY_IN
poll_mode_RDONLY_IN_head() { }
poll_mode_RDONLY_IN_body() {
	h_audio poll_mode_RDONLY_IN
}

atf_test_case poll_mode_RDONLY_OUT
poll_mode_RDONLY_OUT_head() { }
poll_mode_RDONLY_OUT_body() {
	h_audio poll_mode_RDONLY_OUT
}

atf_test_case poll_mode_RDONLY_INOUT
poll_mode_RDONLY_INOUT_head() { }
poll_mode_RDONLY_INOUT_body() {
	h_audio poll_mode_RDONLY_INOUT
}

atf_test_case poll_mode_WRONLY_IN
poll_mode_WRONLY_IN_head() { }
poll_mode_WRONLY_IN_body() {
	h_audio poll_mode_WRONLY_IN
}

atf_test_case poll_mode_WRONLY_OUT
poll_mode_WRONLY_OUT_head() { }
poll_mode_WRONLY_OUT_body() {
	h_audio poll_mode_WRONLY_OUT
}

atf_test_case poll_mode_WRONLY_INOUT
poll_mode_WRONLY_INOUT_head() { }
poll_mode_WRONLY_INOUT_body() {
	h_audio poll_mode_WRONLY_INOUT
}

atf_test_case poll_mode_RDWR_IN
poll_mode_RDWR_IN_head() { }
poll_mode_RDWR_IN_body() {
	h_audio poll_mode_RDWR_IN
}

atf_test_case poll_mode_RDWR_OUT
poll_mode_RDWR_OUT_head() { }
poll_mode_RDWR_OUT_body() {
	h_audio poll_mode_RDWR_OUT
}

atf_test_case poll_mode_RDWR_INOUT
poll_mode_RDWR_INOUT_head() { }
poll_mode_RDWR_INOUT_body() {
	h_audio poll_mode_RDWR_INOUT
}

atf_test_case poll_out_empty
poll_out_empty_head() { }
poll_out_empty_body() {
	h_audio poll_out_empty
}

atf_test_case poll_out_full
poll_out_full_head() { }
poll_out_full_body() {
	h_audio poll_out_full
}

atf_test_case poll_out_hiwat
poll_out_hiwat_head() { }
poll_out_hiwat_body() {
	h_audio poll_out_hiwat
}

atf_test_case poll_in_simul
poll_in_simul_head() { }
poll_in_simul_body() {
	h_audio poll_in_simul
}

atf_test_case kqueue_mode_RDONLY_READ
kqueue_mode_RDONLY_READ_head() { }
kqueue_mode_RDONLY_READ_body() {
	h_audio kqueue_mode_RDONLY_READ
}

atf_test_case kqueue_mode_RDONLY_WRITE
kqueue_mode_RDONLY_WRITE_head() { }
kqueue_mode_RDONLY_WRITE_body() {
	h_audio kqueue_mode_RDONLY_WRITE
}

atf_test_case kqueue_mode_WRONLY_READ
kqueue_mode_WRONLY_READ_head() { }
kqueue_mode_WRONLY_READ_body() {
	h_audio kqueue_mode_WRONLY_READ
}

atf_test_case kqueue_mode_WRONLY_WRITE
kqueue_mode_WRONLY_WRITE_head() { }
kqueue_mode_WRONLY_WRITE_body() {
	h_audio kqueue_mode_WRONLY_WRITE
}

atf_test_case kqueue_mode_RDWR_READ
kqueue_mode_RDWR_READ_head() { }
kqueue_mode_RDWR_READ_body() {
	h_audio kqueue_mode_RDWR_READ
}

atf_test_case kqueue_mode_RDWR_WRITE
kqueue_mode_RDWR_WRITE_head() { }
kqueue_mode_RDWR_WRITE_body() {
	h_audio kqueue_mode_RDWR_WRITE
}

atf_test_case kqueue_empty
kqueue_empty_head() { }
kqueue_empty_body() {
	h_audio kqueue_empty
}

atf_test_case kqueue_full
kqueue_full_head() { }
kqueue_full_body() {
	h_audio kqueue_full
}

atf_test_case kqueue_hiwat
kqueue_hiwat_head() { }
kqueue_hiwat_body() {
	h_audio kqueue_hiwat
}

atf_test_case ioctl_while_write
ioctl_while_write_head() { }
ioctl_while_write_body() {
	h_audio ioctl_while_write
}

atf_test_case FIOASYNC_reset
FIOASYNC_reset_head() { }
FIOASYNC_reset_body() {
	h_audio FIOASYNC_reset
}

atf_test_case FIOASYNC_play_signal
FIOASYNC_play_signal_head() { }
FIOASYNC_play_signal_body() {
	h_audio FIOASYNC_play_signal
}

atf_test_case FIOASYNC_rec_signal
FIOASYNC_rec_signal_head() { }
FIOASYNC_rec_signal_body() {
	h_audio FIOASYNC_rec_signal
}

atf_test_case AUDIO_WSEEK
AUDIO_WSEEK_head() { }
AUDIO_WSEEK_body() {
	h_audio AUDIO_WSEEK
}

atf_test_case AUDIO_SETFD_RDONLY
AUDIO_SETFD_RDONLY_head() { }
AUDIO_SETFD_RDONLY_body() {
	h_audio AUDIO_SETFD_RDONLY
}

atf_test_case AUDIO_SETFD_WRONLY
AUDIO_SETFD_WRONLY_head() { }
AUDIO_SETFD_WRONLY_body() {
	h_audio AUDIO_SETFD_WRONLY
}

atf_test_case AUDIO_SETFD_RDWR
AUDIO_SETFD_RDWR_head() { }
AUDIO_SETFD_RDWR_body() {
	h_audio AUDIO_SETFD_RDWR
}

atf_test_case AUDIO_GETINFO_eof
AUDIO_GETINFO_eof_head() { }
AUDIO_GETINFO_eof_body() {
	h_audio AUDIO_GETINFO_eof
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_0
AUDIO_SETINFO_mode_RDONLY_0_head() { }
AUDIO_SETINFO_mode_RDONLY_0_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_0
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_1
AUDIO_SETINFO_mode_RDONLY_1_head() { }
AUDIO_SETINFO_mode_RDONLY_1_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_1
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_2
AUDIO_SETINFO_mode_RDONLY_2_head() { }
AUDIO_SETINFO_mode_RDONLY_2_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_2
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_3
AUDIO_SETINFO_mode_RDONLY_3_head() { }
AUDIO_SETINFO_mode_RDONLY_3_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_3
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_4
AUDIO_SETINFO_mode_RDONLY_4_head() { }
AUDIO_SETINFO_mode_RDONLY_4_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_4
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_5
AUDIO_SETINFO_mode_RDONLY_5_head() { }
AUDIO_SETINFO_mode_RDONLY_5_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_5
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_6
AUDIO_SETINFO_mode_RDONLY_6_head() { }
AUDIO_SETINFO_mode_RDONLY_6_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_6
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_7
AUDIO_SETINFO_mode_RDONLY_7_head() { }
AUDIO_SETINFO_mode_RDONLY_7_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_7
}

atf_test_case AUDIO_SETINFO_mode_RDONLY_8
AUDIO_SETINFO_mode_RDONLY_8_head() { }
AUDIO_SETINFO_mode_RDONLY_8_body() {
	h_audio AUDIO_SETINFO_mode_RDONLY_8
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_0
AUDIO_SETINFO_mode_WRONLY_0_head() { }
AUDIO_SETINFO_mode_WRONLY_0_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_0
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_1
AUDIO_SETINFO_mode_WRONLY_1_head() { }
AUDIO_SETINFO_mode_WRONLY_1_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_1
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_2
AUDIO_SETINFO_mode_WRONLY_2_head() { }
AUDIO_SETINFO_mode_WRONLY_2_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_2
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_3
AUDIO_SETINFO_mode_WRONLY_3_head() { }
AUDIO_SETINFO_mode_WRONLY_3_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_3
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_4
AUDIO_SETINFO_mode_WRONLY_4_head() { }
AUDIO_SETINFO_mode_WRONLY_4_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_4
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_5
AUDIO_SETINFO_mode_WRONLY_5_head() { }
AUDIO_SETINFO_mode_WRONLY_5_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_5
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_6
AUDIO_SETINFO_mode_WRONLY_6_head() { }
AUDIO_SETINFO_mode_WRONLY_6_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_6
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_7
AUDIO_SETINFO_mode_WRONLY_7_head() { }
AUDIO_SETINFO_mode_WRONLY_7_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_7
}

atf_test_case AUDIO_SETINFO_mode_WRONLY_8
AUDIO_SETINFO_mode_WRONLY_8_head() { }
AUDIO_SETINFO_mode_WRONLY_8_body() {
	h_audio AUDIO_SETINFO_mode_WRONLY_8
}

atf_test_case AUDIO_SETINFO_mode_RDWR_0
AUDIO_SETINFO_mode_RDWR_0_head() { }
AUDIO_SETINFO_mode_RDWR_0_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_0
}

atf_test_case AUDIO_SETINFO_mode_RDWR_1
AUDIO_SETINFO_mode_RDWR_1_head() { }
AUDIO_SETINFO_mode_RDWR_1_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_1
}

atf_test_case AUDIO_SETINFO_mode_RDWR_2
AUDIO_SETINFO_mode_RDWR_2_head() { }
AUDIO_SETINFO_mode_RDWR_2_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_2
}

atf_test_case AUDIO_SETINFO_mode_RDWR_3
AUDIO_SETINFO_mode_RDWR_3_head() { }
AUDIO_SETINFO_mode_RDWR_3_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_3
}

atf_test_case AUDIO_SETINFO_mode_RDWR_4
AUDIO_SETINFO_mode_RDWR_4_head() { }
AUDIO_SETINFO_mode_RDWR_4_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_4
}

atf_test_case AUDIO_SETINFO_mode_RDWR_5
AUDIO_SETINFO_mode_RDWR_5_head() { }
AUDIO_SETINFO_mode_RDWR_5_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_5
}

atf_test_case AUDIO_SETINFO_mode_RDWR_6
AUDIO_SETINFO_mode_RDWR_6_head() { }
AUDIO_SETINFO_mode_RDWR_6_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_6
}

atf_test_case AUDIO_SETINFO_mode_RDWR_7
AUDIO_SETINFO_mode_RDWR_7_head() { }
AUDIO_SETINFO_mode_RDWR_7_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_7
}

atf_test_case AUDIO_SETINFO_mode_RDWR_8
AUDIO_SETINFO_mode_RDWR_8_head() { }
AUDIO_SETINFO_mode_RDWR_8_body() {
	h_audio AUDIO_SETINFO_mode_RDWR_8
}

atf_test_case AUDIO_SETINFO_params_set_RDONLY_0
AUDIO_SETINFO_params_set_RDONLY_0_head() { }
AUDIO_SETINFO_params_set_RDONLY_0_body() {
	h_audio AUDIO_SETINFO_params_set_RDONLY_0
}

atf_test_case AUDIO_SETINFO_params_set_RDONLY_1
AUDIO_SETINFO_params_set_RDONLY_1_head() { }
AUDIO_SETINFO_params_set_RDONLY_1_body() {
	h_audio AUDIO_SETINFO_params_set_RDONLY_1
}

atf_test_case AUDIO_SETINFO_params_set_WRONLY_0
AUDIO_SETINFO_params_set_WRONLY_0_head() { }
AUDIO_SETINFO_params_set_WRONLY_0_body() {
	h_audio AUDIO_SETINFO_params_set_WRONLY_0
}

atf_test_case AUDIO_SETINFO_params_set_WRONLY_1
AUDIO_SETINFO_params_set_WRONLY_1_head() { }
AUDIO_SETINFO_params_set_WRONLY_1_body() {
	h_audio AUDIO_SETINFO_params_set_WRONLY_1
}

atf_test_case AUDIO_SETINFO_params_set_WRONLY_2
AUDIO_SETINFO_params_set_WRONLY_2_head() { }
AUDIO_SETINFO_params_set_WRONLY_2_body() {
	h_audio AUDIO_SETINFO_params_set_WRONLY_2
}

atf_test_case AUDIO_SETINFO_params_set_WRONLY_3
AUDIO_SETINFO_params_set_WRONLY_3_head() { }
AUDIO_SETINFO_params_set_WRONLY_3_body() {
	h_audio AUDIO_SETINFO_params_set_WRONLY_3
}

atf_test_case AUDIO_SETINFO_params_set_RDWR_0
AUDIO_SETINFO_params_set_RDWR_0_head() { }
AUDIO_SETINFO_params_set_RDWR_0_body() {
	h_audio AUDIO_SETINFO_params_set_RDWR_0
}

atf_test_case AUDIO_SETINFO_params_set_RDWR_1
AUDIO_SETINFO_params_set_RDWR_1_head() { }
AUDIO_SETINFO_params_set_RDWR_1_body() {
	h_audio AUDIO_SETINFO_params_set_RDWR_1
}

atf_test_case AUDIO_SETINFO_params_set_RDWR_2
AUDIO_SETINFO_params_set_RDWR_2_head() { }
AUDIO_SETINFO_params_set_RDWR_2_body() {
	h_audio AUDIO_SETINFO_params_set_RDWR_2
}

atf_test_case AUDIO_SETINFO_params_set_RDWR_3
AUDIO_SETINFO_params_set_RDWR_3_head() { }
AUDIO_SETINFO_params_set_RDWR_3_body() {
	h_audio AUDIO_SETINFO_params_set_RDWR_3
}

atf_test_case AUDIO_SETINFO_params_simul
AUDIO_SETINFO_params_simul_head() { }
AUDIO_SETINFO_params_simul_body() {
	h_audio AUDIO_SETINFO_params_simul
}

atf_test_case AUDIO_SETINFO_pause_RDONLY_0
AUDIO_SETINFO_pause_RDONLY_0_head() { }
AUDIO_SETINFO_pause_RDONLY_0_body() {
	h_audio AUDIO_SETINFO_pause_RDONLY_0
}

atf_test_case AUDIO_SETINFO_pause_RDONLY_1
AUDIO_SETINFO_pause_RDONLY_1_head() { }
AUDIO_SETINFO_pause_RDONLY_1_body() {
	h_audio AUDIO_SETINFO_pause_RDONLY_1
}

atf_test_case AUDIO_SETINFO_pause_WRONLY_0
AUDIO_SETINFO_pause_WRONLY_0_head() { }
AUDIO_SETINFO_pause_WRONLY_0_body() {
	h_audio AUDIO_SETINFO_pause_WRONLY_0
}

atf_test_case AUDIO_SETINFO_pause_WRONLY_1
AUDIO_SETINFO_pause_WRONLY_1_head() { }
AUDIO_SETINFO_pause_WRONLY_1_body() {
	h_audio AUDIO_SETINFO_pause_WRONLY_1
}

atf_test_case AUDIO_SETINFO_pause_WRONLY_2
AUDIO_SETINFO_pause_WRONLY_2_head() { }
AUDIO_SETINFO_pause_WRONLY_2_body() {
	h_audio AUDIO_SETINFO_pause_WRONLY_2
}

atf_test_case AUDIO_SETINFO_pause_WRONLY_3
AUDIO_SETINFO_pause_WRONLY_3_head() { }
AUDIO_SETINFO_pause_WRONLY_3_body() {
	h_audio AUDIO_SETINFO_pause_WRONLY_3
}

atf_test_case AUDIO_SETINFO_pause_RDWR_0
AUDIO_SETINFO_pause_RDWR_0_head() { }
AUDIO_SETINFO_pause_RDWR_0_body() {
	h_audio AUDIO_SETINFO_pause_RDWR_0
}

atf_test_case AUDIO_SETINFO_pause_RDWR_1
AUDIO_SETINFO_pause_RDWR_1_head() { }
AUDIO_SETINFO_pause_RDWR_1_body() {
	h_audio AUDIO_SETINFO_pause_RDWR_1
}

atf_test_case AUDIO_SETINFO_pause_RDWR_2
AUDIO_SETINFO_pause_RDWR_2_head() { }
AUDIO_SETINFO_pause_RDWR_2_body() {
	h_audio AUDIO_SETINFO_pause_RDWR_2
}

atf_test_case AUDIO_SETINFO_pause_RDWR_3
AUDIO_SETINFO_pause_RDWR_3_head() { }
AUDIO_SETINFO_pause_RDWR_3_body() {
	h_audio AUDIO_SETINFO_pause_RDWR_3
}

atf_test_case AUDIO_SETINFO_gain
AUDIO_SETINFO_gain_head() { }
AUDIO_SETINFO_gain_body() {
	h_audio AUDIO_SETINFO_gain
}

atf_test_case AUDIO_GETENC_range
AUDIO_GETENC_range_head() { }
AUDIO_GETENC_range_body() {
	h_audio AUDIO_GETENC_range
}

atf_test_case AUDIO_GETENC_error
AUDIO_GETENC_error_head() { }
AUDIO_GETENC_error_body() {
	h_audio AUDIO_GETENC_error
}

atf_test_case AUDIO_ERROR_RDONLY
AUDIO_ERROR_RDONLY_head() { }
AUDIO_ERROR_RDONLY_body() {
	h_audio AUDIO_ERROR_RDONLY
}

atf_test_case AUDIO_ERROR_WRONLY
AUDIO_ERROR_WRONLY_head() { }
AUDIO_ERROR_WRONLY_body() {
	h_audio AUDIO_ERROR_WRONLY
}

atf_test_case AUDIO_ERROR_RDWR
AUDIO_ERROR_RDWR_head() { }
AUDIO_ERROR_RDWR_body() {
	h_audio AUDIO_ERROR_RDWR
}

atf_test_case audioctl_open_1_RDONLY_RDONLY
audioctl_open_1_RDONLY_RDONLY_head() { }
audioctl_open_1_RDONLY_RDONLY_body() {
	h_audio audioctl_open_1_RDONLY_RDONLY
}

atf_test_case audioctl_open_1_RDONLY_RWONLY
audioctl_open_1_RDONLY_RWONLY_head() { }
audioctl_open_1_RDONLY_RWONLY_body() {
	h_audio audioctl_open_1_RDONLY_RWONLY
}

atf_test_case audioctl_open_1_RDONLY_RDWR
audioctl_open_1_RDONLY_RDWR_head() { }
audioctl_open_1_RDONLY_RDWR_body() {
	h_audio audioctl_open_1_RDONLY_RDWR
}

atf_test_case audioctl_open_1_WRONLY_RDONLY
audioctl_open_1_WRONLY_RDONLY_head() { }
audioctl_open_1_WRONLY_RDONLY_body() {
	h_audio audioctl_open_1_WRONLY_RDONLY
}

atf_test_case audioctl_open_1_WRONLY_RWONLY
audioctl_open_1_WRONLY_RWONLY_head() { }
audioctl_open_1_WRONLY_RWONLY_body() {
	h_audio audioctl_open_1_WRONLY_RWONLY
}

atf_test_case audioctl_open_1_WRONLY_RDWR
audioctl_open_1_WRONLY_RDWR_head() { }
audioctl_open_1_WRONLY_RDWR_body() {
	h_audio audioctl_open_1_WRONLY_RDWR
}

atf_test_case audioctl_open_1_RDWR_RDONLY
audioctl_open_1_RDWR_RDONLY_head() { }
audioctl_open_1_RDWR_RDONLY_body() {
	h_audio audioctl_open_1_RDWR_RDONLY
}

atf_test_case audioctl_open_1_RDWR_RWONLY
audioctl_open_1_RDWR_RWONLY_head() { }
audioctl_open_1_RDWR_RWONLY_body() {
	h_audio audioctl_open_1_RDWR_RWONLY
}

atf_test_case audioctl_open_1_RDWR_RDWR
audioctl_open_1_RDWR_RDWR_head() { }
audioctl_open_1_RDWR_RDWR_body() {
	h_audio audioctl_open_1_RDWR_RDWR
}

atf_test_case audioctl_open_2_RDONLY_RDONLY
audioctl_open_2_RDONLY_RDONLY_head() { }
audioctl_open_2_RDONLY_RDONLY_body() {
	h_audio audioctl_open_2_RDONLY_RDONLY
}

atf_test_case audioctl_open_2_RDONLY_RWONLY
audioctl_open_2_RDONLY_RWONLY_head() { }
audioctl_open_2_RDONLY_RWONLY_body() {
	h_audio audioctl_open_2_RDONLY_RWONLY
}

atf_test_case audioctl_open_2_RDONLY_RDWR
audioctl_open_2_RDONLY_RDWR_head() { }
audioctl_open_2_RDONLY_RDWR_body() {
	h_audio audioctl_open_2_RDONLY_RDWR
}

atf_test_case audioctl_open_2_WRONLY_RDONLY
audioctl_open_2_WRONLY_RDONLY_head() { }
audioctl_open_2_WRONLY_RDONLY_body() {
	h_audio audioctl_open_2_WRONLY_RDONLY
}

atf_test_case audioctl_open_2_WRONLY_RWONLY
audioctl_open_2_WRONLY_RWONLY_head() { }
audioctl_open_2_WRONLY_RWONLY_body() {
	h_audio audioctl_open_2_WRONLY_RWONLY
}

atf_test_case audioctl_open_2_WRONLY_RDWR
audioctl_open_2_WRONLY_RDWR_head() { }
audioctl_open_2_WRONLY_RDWR_body() {
	h_audio audioctl_open_2_WRONLY_RDWR
}

atf_test_case audioctl_open_2_RDWR_RDONLY
audioctl_open_2_RDWR_RDONLY_head() { }
audioctl_open_2_RDWR_RDONLY_body() {
	h_audio audioctl_open_2_RDWR_RDONLY
}

atf_test_case audioctl_open_2_RDWR_RWONLY
audioctl_open_2_RDWR_RWONLY_head() { }
audioctl_open_2_RDWR_RWONLY_body() {
	h_audio audioctl_open_2_RDWR_RWONLY
}

atf_test_case audioctl_open_2_RDWR_RDWR
audioctl_open_2_RDWR_RDWR_head() { }
audioctl_open_2_RDWR_RDWR_body() {
	h_audio audioctl_open_2_RDWR_RDWR
}

atf_test_case audioctl_open_simul
audioctl_open_simul_head() { }
audioctl_open_simul_body() {
	h_audio audioctl_open_simul
}

atf_test_case audioctl_open_multiuser0_audio1
audioctl_open_multiuser0_audio1_head() { }
audioctl_open_multiuser0_audio1_body() {
	h_audio audioctl_open_multiuser0_audio1
}

atf_test_case audioctl_open_multiuser1_audio1
audioctl_open_multiuser1_audio1_head() { }
audioctl_open_multiuser1_audio1_body() {
	h_audio audioctl_open_multiuser1_audio1
}

atf_test_case audioctl_open_multiuser0_audio2
audioctl_open_multiuser0_audio2_head() { }
audioctl_open_multiuser0_audio2_body() {
	h_audio audioctl_open_multiuser0_audio2
}

atf_test_case audioctl_open_multiuser1_audio2
audioctl_open_multiuser1_audio2_head() { }
audioctl_open_multiuser1_audio2_body() {
	h_audio audioctl_open_multiuser1_audio2
}

atf_test_case audioctl_open_multiuser0_audioctl
audioctl_open_multiuser0_audioctl_head() { }
audioctl_open_multiuser0_audioctl_body() {
	h_audio audioctl_open_multiuser0_audioctl
}

atf_test_case audioctl_open_multiuser1_audioctl
audioctl_open_multiuser1_audioctl_head() { }
audioctl_open_multiuser1_audioctl_body() {
	h_audio audioctl_open_multiuser1_audioctl
}

atf_test_case audioctl_rw_RDONLY
audioctl_rw_RDONLY_head() { }
audioctl_rw_RDONLY_body() {
	h_audio audioctl_rw_RDONLY
}

atf_test_case audioctl_rw_WRONLY
audioctl_rw_WRONLY_head() { }
audioctl_rw_WRONLY_body() {
	h_audio audioctl_rw_WRONLY
}

atf_test_case audioctl_rw_RDWR
audioctl_rw_RDWR_head() { }
audioctl_rw_RDWR_body() {
	h_audio audioctl_rw_RDWR
}

atf_test_case audioctl_poll
audioctl_poll_head() { }
audioctl_poll_body() {
	h_audio audioctl_poll
}

atf_test_case audioctl_kqueue
audioctl_kqueue_head() { }
audioctl_kqueue_body() {
	h_audio audioctl_kqueue
}

atf_init_test_cases() {
	atf_add_test_case open_mode_RDONLY
	atf_add_test_case open_mode_WRONLY
	atf_add_test_case open_mode_RDWR
	atf_add_test_case open_audio_RDONLY
	atf_add_test_case open_audio_WRONLY
	atf_add_test_case open_audio_RDWR
	atf_add_test_case open_sound_RDONLY
	atf_add_test_case open_sound_WRONLY
	atf_add_test_case open_sound_RDWR
	atf_add_test_case open_audioctl_RDONLY
	atf_add_test_case open_audioctl_WRONLY
	atf_add_test_case open_audioctl_RDWR
	atf_add_test_case open_sound_sticky
	atf_add_test_case open_audioctl_sticky
	atf_add_test_case open_simul_RDONLY_RDONLY
	atf_add_test_case open_simul_RDONLY_WRONLY
	atf_add_test_case open_simul_RDONLY_RDWR
	atf_add_test_case open_simul_WRONLY_RDONLY
	atf_add_test_case open_simul_WRONLY_WRONLY
	atf_add_test_case open_simul_WRONLY_RDWR
	atf_add_test_case open_simul_RDWR_RDONLY
	atf_add_test_case open_simul_RDWR_WRONLY
	atf_add_test_case open_simul_RDWR_RDWR
	atf_add_test_case open_multiuser_0
	atf_add_test_case open_multiuser_1
	atf_add_test_case write_PLAY_ALL
	atf_add_test_case write_PLAY
	atf_add_test_case read
	atf_add_test_case rept_write
	atf_add_test_case rept_read
	atf_add_test_case rdwr_fallback_RDONLY
	atf_add_test_case rdwr_fallback_WRONLY
	atf_add_test_case rdwr_fallback_RDWR
	atf_add_test_case rdwr_two_RDONLY_RDONLY
	atf_add_test_case rdwr_two_RDONLY_WRONLY
	atf_add_test_case rdwr_two_RDONLY_RDWR
	atf_add_test_case rdwr_two_WRONLY_RDONLY
	atf_add_test_case rdwr_two_WRONLY_WRONLY
	atf_add_test_case rdwr_two_WRONLY_RDWR
	atf_add_test_case rdwr_two_RDWR_RDONLY
	atf_add_test_case rdwr_two_RDWR_WRONLY
	atf_add_test_case rdwr_two_RDWR_RDWR
	atf_add_test_case rdwr_simul
	atf_add_test_case drain_incomplete
	atf_add_test_case drain_pause
	atf_add_test_case drain_onrec
	atf_add_test_case mmap_mode_RDONLY_NONE
	atf_add_test_case mmap_mode_RDONLY_READ
	atf_add_test_case mmap_mode_RDONLY_WRITE
	atf_add_test_case mmap_mode_RDONLY_READWRITE
	atf_add_test_case mmap_mode_WRONLY_NONE
	atf_add_test_case mmap_mode_WRONLY_READ
	atf_add_test_case mmap_mode_WRONLY_WRITE
	atf_add_test_case mmap_mode_WRONLY_READWRITE
	atf_add_test_case mmap_mode_RDWR_NONE
	atf_add_test_case mmap_mode_RDWR_READ
	atf_add_test_case mmap_mode_RDWR_WRITE
	atf_add_test_case mmap_mode_RDWR_READWRITE
	atf_add_test_case mmap_len
	atf_add_test_case mmap_twice
	atf_add_test_case mmap_multi
	atf_add_test_case poll_mode_RDONLY_IN
	atf_add_test_case poll_mode_RDONLY_OUT
	atf_add_test_case poll_mode_RDONLY_INOUT
	atf_add_test_case poll_mode_WRONLY_IN
	atf_add_test_case poll_mode_WRONLY_OUT
	atf_add_test_case poll_mode_WRONLY_INOUT
	atf_add_test_case poll_mode_RDWR_IN
	atf_add_test_case poll_mode_RDWR_OUT
	atf_add_test_case poll_mode_RDWR_INOUT
	atf_add_test_case poll_out_empty
	atf_add_test_case poll_out_full
	atf_add_test_case poll_out_hiwat
	atf_add_test_case poll_in_simul
	atf_add_test_case kqueue_mode_RDONLY_READ
	atf_add_test_case kqueue_mode_RDONLY_WRITE
	atf_add_test_case kqueue_mode_WRONLY_READ
	atf_add_test_case kqueue_mode_WRONLY_WRITE
	atf_add_test_case kqueue_mode_RDWR_READ
	atf_add_test_case kqueue_mode_RDWR_WRITE
	atf_add_test_case kqueue_empty
	atf_add_test_case kqueue_full
	atf_add_test_case kqueue_hiwat
	atf_add_test_case ioctl_while_write
	atf_add_test_case FIOASYNC_reset
	atf_add_test_case FIOASYNC_play_signal
	atf_add_test_case FIOASYNC_rec_signal
	atf_add_test_case AUDIO_WSEEK
	atf_add_test_case AUDIO_SETFD_RDONLY
	atf_add_test_case AUDIO_SETFD_WRONLY
	atf_add_test_case AUDIO_SETFD_RDWR
	atf_add_test_case AUDIO_GETINFO_eof
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_0
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_1
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_2
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_3
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_4
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_5
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_6
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_7
	atf_add_test_case AUDIO_SETINFO_mode_RDONLY_8
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_0
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_1
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_2
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_3
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_4
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_5
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_6
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_7
	atf_add_test_case AUDIO_SETINFO_mode_WRONLY_8
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_0
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_1
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_2
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_3
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_4
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_5
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_6
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_7
	atf_add_test_case AUDIO_SETINFO_mode_RDWR_8
	atf_add_test_case AUDIO_SETINFO_params_set_RDONLY_0
	atf_add_test_case AUDIO_SETINFO_params_set_RDONLY_1
	atf_add_test_case AUDIO_SETINFO_params_set_WRONLY_0
	atf_add_test_case AUDIO_SETINFO_params_set_WRONLY_1
	atf_add_test_case AUDIO_SETINFO_params_set_WRONLY_2
	atf_add_test_case AUDIO_SETINFO_params_set_WRONLY_3
	atf_add_test_case AUDIO_SETINFO_params_set_RDWR_0
	atf_add_test_case AUDIO_SETINFO_params_set_RDWR_1
	atf_add_test_case AUDIO_SETINFO_params_set_RDWR_2
	atf_add_test_case AUDIO_SETINFO_params_set_RDWR_3
	atf_add_test_case AUDIO_SETINFO_params_simul
	atf_add_test_case AUDIO_SETINFO_pause_RDONLY_0
	atf_add_test_case AUDIO_SETINFO_pause_RDONLY_1
	atf_add_test_case AUDIO_SETINFO_pause_WRONLY_0
	atf_add_test_case AUDIO_SETINFO_pause_WRONLY_1
	atf_add_test_case AUDIO_SETINFO_pause_WRONLY_2
	atf_add_test_case AUDIO_SETINFO_pause_WRONLY_3
	atf_add_test_case AUDIO_SETINFO_pause_RDWR_0
	atf_add_test_case AUDIO_SETINFO_pause_RDWR_1
	atf_add_test_case AUDIO_SETINFO_pause_RDWR_2
	atf_add_test_case AUDIO_SETINFO_pause_RDWR_3
	atf_add_test_case AUDIO_SETINFO_gain
	atf_add_test_case AUDIO_GETENC_range
	atf_add_test_case AUDIO_GETENC_error
	atf_add_test_case AUDIO_ERROR_RDONLY
	atf_add_test_case AUDIO_ERROR_WRONLY
	atf_add_test_case AUDIO_ERROR_RDWR
	atf_add_test_case audioctl_open_1_RDONLY_RDONLY
	atf_add_test_case audioctl_open_1_RDONLY_RWONLY
	atf_add_test_case audioctl_open_1_RDONLY_RDWR
	atf_add_test_case audioctl_open_1_WRONLY_RDONLY
	atf_add_test_case audioctl_open_1_WRONLY_RWONLY
	atf_add_test_case audioctl_open_1_WRONLY_RDWR
	atf_add_test_case audioctl_open_1_RDWR_RDONLY
	atf_add_test_case audioctl_open_1_RDWR_RWONLY
	atf_add_test_case audioctl_open_1_RDWR_RDWR
	atf_add_test_case audioctl_open_2_RDONLY_RDONLY
	atf_add_test_case audioctl_open_2_RDONLY_RWONLY
	atf_add_test_case audioctl_open_2_RDONLY_RDWR
	atf_add_test_case audioctl_open_2_WRONLY_RDONLY
	atf_add_test_case audioctl_open_2_WRONLY_RWONLY
	atf_add_test_case audioctl_open_2_WRONLY_RDWR
	atf_add_test_case audioctl_open_2_RDWR_RDONLY
	atf_add_test_case audioctl_open_2_RDWR_RWONLY
	atf_add_test_case audioctl_open_2_RDWR_RDWR
	atf_add_test_case audioctl_open_simul
	atf_add_test_case audioctl_open_multiuser0_audio1
	atf_add_test_case audioctl_open_multiuser1_audio1
	atf_add_test_case audioctl_open_multiuser0_audio2
	atf_add_test_case audioctl_open_multiuser1_audio2
	atf_add_test_case audioctl_open_multiuser0_audioctl
	atf_add_test_case audioctl_open_multiuser1_audioctl
	atf_add_test_case audioctl_rw_RDONLY
	atf_add_test_case audioctl_rw_WRONLY
	atf_add_test_case audioctl_rw_RDWR
	atf_add_test_case audioctl_poll
	atf_add_test_case audioctl_kqueue
}
