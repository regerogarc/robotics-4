task play_sound()
{
	int timing_multiplier = 1;
	playTone(739.99,384 * timing_multiplier);
	playTone(554.37,384 * timing_multiplier);
	playTone(493.88,384 * timing_multiplier);
	playTone(440.0,192 * timing_multiplier);
	playTone(369.99,193 * timing_multiplier);
	delay(191 * ms_conversion);
	playTone(369.99,193 * timing_multiplier);
	delay(191 * ms_conversion);
	playTone(369.99,193 * timing_multiplier);
	delay(191 * ms_conversion);
	playTone(369.99,193 * timing_multiplier);
	delay(191 * ms_conversion);
	playTone(369.99,193 * timing_multiplier);
	delay(0 * ms_conversion);
}