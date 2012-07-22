/*--------------------------------------------------------------------------------*
 *                                                                                *
 * This material is trade secret owned by VoiceAge Corporation                    *
 * and is strictly confidential and shall remain as such.                         *
 *                                                                                *
 * Copyright © 1995-2001 VoiceAge Corporation. All Rights Reserved. No part of    *
 * this material may be reproduced, stored in a retrieval system, or transmitted, *
 * in any form or by any means, including, but not limited to, photocopying,      *
 *  electronic, mechanical, recording, or otherwise, without the prior written    *
 * permission of VoiceAge Corporation.                                            *
 *                                                                                *
 * This material is subject to continuous developments and improvements. All      *
 * warranties implied or expressed, including but not limited to implied          *
 * warranties of merchantability, or fitness for purpose, are excluded.           *
 *                                                                                *
 * ACELP and VoiceAge are registered trademark and trademark of VoiceAge          *
 * Corporation in Canada and / or other countries. Any unauthorized use is        *
 * strictly prohibited.                                                           *
 *                                                                                *
 *--------------------------------------------------------------------------------*
 *                                                                                *
 * VoiceAge Corporation                                                           *
 * 750, Chemin Lucerne                                                            *
 * Suite 250                                                                      *
 * Ville Mont-Royal (Quebec)                                                      *
 * Canada, H3R 2H6                                                                *
 *                                                                                *
 * Tel. (514) 737-4940, fax. (514) 908-2037                                       *
 *                                                                                *
 *--------------------------------------------------------------------------------*
 *
 *--------------------------------------------------------------------------------*
 *                         va_g729_decoder.c                                     *
 *                         ~~~~~~~~~~~~~~~~~~                                     *
 * Example of the ITU G.729 CS-ACELP 8.0kbps speech decoder                       *
 *                                                                                *
 *--------------------------------------------------------------------------------*/

#include "stdio.h"
#include "va_g729.h"
#include <time.h>

void main(int argc, char *argv[])
{

	int nb_frame;

	clock_t start, finish;
	double duration;

	FILE* fp_in;
	FILE* fp_out;

	unsigned char	serial[L_FRAME_COMPRESSED];
	short			synth[L_FRAME];
	int				bfi;

	printf("\n**************         VoiceAge Corporation        **************");
	printf("\n");
	printf("\n-------------      G729 floating-point Decoder      ------------");
	printf("\n");

	/*-----------------------------------------------------------------------*
	 * Open all files.                                                       *
	 *-----------------------------------------------------------------------*/

	if (argc != 3)
	{
		printf("Usage: %s infile outfile\n", argv[0]);
		return;
	}

	if ( (fp_in = fopen(argv[1], "rb")) == NULL)
	{
		printf("\nError opening input file %s!", argv[1]);
		return;
	} 

	if ( (fp_out = fopen(argv[2], "wb")) == NULL)
	{
		printf("\nError opening output file %s!", argv[2]);
		return;
	}

	/*-----------------------------------------------------------------------*
	 * Decode                                                                *
	 *-----------------------------------------------------------------------*/

	va_g729a_init_decoder();

	nb_frame = 0;
  	start = clock();

	while (fread(serial, sizeof(char), L_FRAME_COMPRESSED, fp_in) == L_FRAME_COMPRESSED) 
	{

		printf("Decode frame %d\r", ++nb_frame);

		/*--------------------------------------------------------------*
		 * Bad frame                                                    *
		 *                                                              *
		 * bfi = 0 to indicate that we have a correct frame             *
		 * bfi = 1 to indicate that we lost the current frame           *
		 *--------------------------------------------------------------*/

		bfi = 0;

		/*--------------------------------------------------------------*
		 * Call the decoder.                                            *
		 *--------------------------------------------------------------*/

		va_g729a_decoder(serial, synth, bfi);

		/*--------------------------------------------------------------*
		 * Output synthesis to disk                                     *
		 *--------------------------------------------------------------*/

		fwrite(synth, sizeof(short), L_FRAME, fp_out);
	}

   finish = clock();
   
   duration = (double)(finish - start) / CLOCKS_PER_SEC;
   printf( "\n%2.1f seconds\n", duration );

	fclose(fp_out);
	fclose(fp_in);

} /* end of main() */
