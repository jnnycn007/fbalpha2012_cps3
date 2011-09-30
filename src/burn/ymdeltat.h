#ifndef __YMDELTAT_H_
#define __YMDELTAT_H_

#define YM_DELTAT_SHIFT    (16)

#define YM_DELTAT_EMULATION_MODE_NORMAL	0
#define YM_DELTAT_EMULATION_MODE_YM2610	1


typedef void (*STATUS_CHANGE_HANDLER)(uint8_t which_chip, uint8_t status_bits);


/* DELTA-T (adpcm type B) struct */
typedef struct deltat_adpcm_state {     /* AT: rearranged and tigntened structure */
	uint8_t	*memory;
	int32_t	*output_pointer;/* pointer of output pointers	*/
	int32_t	*pan;			/* pan : &output_pointer[pan]	*/
	double	freqbase;
#if 0
	double	write_time;		/* Y8950: 10 cycles of main clock; YM2608: 20 cycles of main clock */
	double	read_time;		/* Y8950: 8 cycles of main clock;  YM2608: 18 cycles of main clock */
#endif
	uint32_t	memory_size;
	int	output_range;
	uint32_t	now_addr;		/* current address		*/
	uint32_t	now_step;		/* currect step			*/
	uint32_t	step;			/* step					*/
	uint32_t	start;			/* start address		*/
	uint32_t	limit;			/* limit address		*/
	uint32_t	end;			/* end address			*/
	uint32_t	delta;			/* delta scale			*/
	int32_t	volume;			/* current volume		*/
	int32_t	acc;			/* shift Measurement value*/
	int32_t	adpcmd;			/* next Forecast		*/
	int32_t	adpcml;			/* current value		*/
	int32_t	prev_acc;		/* leveling value		*/
	uint8_t	now_data;		/* current rom data		*/
	uint8_t	CPU_data;		/* current data	from reg 08	*/
	uint8_t	portstate;		/* port status			*/
	uint8_t	control2;		/* control reg: SAMPLE, DA/AD, RAM TYPE (x8bit / x1bit), ROM/RAM */
	uint8_t	portshift;		/* address bits shift-left:
							** 8 for YM2610,
							** 5 for Y8950 and YM2608 */

	uint8_t DRAMportshift;	/* address bits shift-right:
							** 0 for ROM and x8bit DRAMs,
							** 3 for x1 DRAMs */

	uint8_t memread;		/* needed for reading/writing external memory */

	/* handlers and parameters for the status flags support */
	STATUS_CHANGE_HANDLER	status_set_handler;
	STATUS_CHANGE_HANDLER	status_reset_handler;

	/* note that different chips have these flags on different
	** bits of the status register
	*/
	uint8_t status_change_which_chip;	/* this chip id */
	uint8_t status_change_EOS_bit;		/* 1 on End Of Sample (record/playback/cycle time of AD/DA converting has passed)*/
	uint8_t status_change_BRDY_bit;		/* 1 after recording 2 datas (2x4bits) or after reading/writing 1 data */
	uint8_t	status_change_ZERO_bit;		/* 1 if silence lasts for more than 290 miliseconds on ADPCM recording */

	/* neither Y8950 nor YM2608 can generate IRQ when PCMBSY bit changes, so instead of above,
	** the statusflag gets ORed with PCM_BSY (below) (on each read of statusflag of Y8950 and YM2608)
	*/
	uint8_t	PCM_BSY;		/* 1 when ADPCM is playing; Y8950/YM2608 only */

	uint8_t	reg[16];		/* adpcm registers		*/
	uint8_t	emulation_mode;	/* which chip we're emulating */
}YM_DELTAT;

/*void YM_DELTAT_BRDY_callback(YM_DELTAT *DELTAT);*/

uint8_t YM_DELTAT_ADPCM_Read(YM_DELTAT *DELTAT);
void YM_DELTAT_ADPCM_Write(YM_DELTAT *DELTAT,int r,int v);
void YM_DELTAT_ADPCM_Reset(YM_DELTAT *DELTAT,int pan,int emulation_mode);
void YM_DELTAT_ADPCM_CALC(YM_DELTAT *DELTAT);

void YM_DELTAT_postload(YM_DELTAT *DELTAT, uint8_t *regs);
void YM_DELTAT_savestate(const char *statename,int num,YM_DELTAT *DELTAT);

#endif