#define ROTR64(x, n) ((x << (64 - n)) | (x >> n))

#define SWAP_ENDIANESS(x) ((((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24)))

if (xlen == 32) {
	uint64_t swap_endianess = (uint64_t) ((insn.i_imm() & 0x800) >> 11);
	uint64_t ROUNDS = (uint64_t) ((insn.i_imm() & 0x700) >> 8);
	uint64_t C = (uint64_t) (insn.i_imm() & 0xFF);

	uint32_t x0_hi = READ_REG(12);
	uint32_t x0_lo = READ_REG(13);
	uint32_t x1_hi = READ_REG(14);
	uint32_t x1_lo = READ_REG(15);
	uint32_t x2_hi = READ_REG(16);
	uint32_t x2_lo = READ_REG(17);
	uint32_t x3_hi = READ_REG(28);
	uint32_t x3_lo = READ_REG(29);
	uint32_t x4_hi = READ_REG(30);
	uint32_t x4_lo = READ_REG(31);

	if (swap_endianess == 1) {
		x0_hi = SWAP_ENDIANESS(x0_hi);
		x0_lo = SWAP_ENDIANESS(x0_lo);
		x1_hi = SWAP_ENDIANESS(x1_hi);
		x1_lo = SWAP_ENDIANESS(x1_lo);
		x2_hi = SWAP_ENDIANESS(x2_hi);
		x2_lo = SWAP_ENDIANESS(x2_lo);
		x3_hi = SWAP_ENDIANESS(x3_hi);
		x3_lo = SWAP_ENDIANESS(x3_lo);
		x4_hi = SWAP_ENDIANESS(x4_hi);
		x4_lo = SWAP_ENDIANESS(x4_lo);
	}


	uint64_t x0 = (uint64_t) (((uint64_t) x0_hi << 32) | x0_lo);
	uint64_t x1 = (uint64_t) (((uint64_t) x1_hi << 32) | x1_lo);
	uint64_t x2 = (uint64_t) (((uint64_t) x2_hi << 32) | x2_lo);
	uint64_t x3 = (uint64_t) (((uint64_t) x3_hi << 32) | x3_lo);
	uint64_t x4 = (uint64_t) (((uint64_t) x4_hi << 32) | x4_lo);


	for (unsigned int r = 0; r < (ROUNDS+1); r++) {
		x2 ^= ((((C & 0xF0) >> 4) - r) << 4) | ((C & 0xF) + r);
		// x2 ^= C;
		// printf(" addition of round constant:", C);
		// substitution layer
		         x0      ^= x4;       // s.x0 ^= s.x4;
		         x4      ^= x3;       // s.x4 ^= s.x3;
		         x2      ^= x1;       // s.x2 ^= s.x1;
		// start of keccak s-box
		uint64_t x0_kec   = ~x0;      // t.x0 = ~s.x0;
		uint64_t x1_kec   = ~x1;      // t.x1 = ~s.x1;
		uint64_t x2_kec   = ~x2;      // t.x2 = ~s.x2;
		uint64_t x3_kec   = ~x3;      // t.x3 = ~s.x3;
		uint64_t x4_kec   = ~x4;      // t.x4 = ~s.x4;
		         x0_kec  &=  x1;      // t.x0 &= s.x1;
		         x1_kec  &=  x2;      // t.x1 &= s.x2;
		         x2_kec  &=  x3;      // t.x2 &= s.x3;
		         x3_kec  &=  x4;      // t.x3 &= s.x4;
		         x4_kec  &=  x0;      // t.x4 &= s.x0;
		             x0  ^= x1_kec;   // s.x0 ^= t.x1;
		             x1  ^= x2_kec;   // s.x1 ^= t.x2;
		             x2  ^= x3_kec;   // s.x2 ^= t.x3;
		             x3  ^= x4_kec;   // s.x3 ^= t.x4;
		             x4  ^= x0_kec;   // s.x4 ^= t.x0;
		// end of keccak s-box
		x1 ^= x0;
		x0 ^= x4;
		x3 ^= x2;
		x2 = ~x2;


		// linear diffusion layer
		x0 ^= ROTR64(x0, 19) ^ ROTR64(x0, 28);
		x1 ^= ROTR64(x1, 61) ^ ROTR64(x1, 39);
		x2 ^= ROTR64(x2,  1) ^ ROTR64(x2,  6);
		x3 ^= ROTR64(x3, 10) ^ ROTR64(x3, 17);
		x4 ^= ROTR64(x4,  7) ^ ROTR64(x4, 41);
	}

	// splitting to 32 bit registers
	x0_hi = (uint32_t) ((x0 >> 32) & 0xFFFFFFFF);
	x0_lo = (uint32_t) (x0         & 0xFFFFFFFF);
	x1_hi = (uint32_t) ((x1 >> 32) & 0xFFFFFFFF);
	x1_lo = (uint32_t) (x1         & 0xFFFFFFFF);
	x2_hi = (uint32_t) ((x2 >> 32) & 0xFFFFFFFF);
	x2_lo = (uint32_t) (x2         & 0xFFFFFFFF);
	x3_hi = (uint32_t) ((x3 >> 32) & 0xFFFFFFFF);
	x3_lo = (uint32_t) (x3         & 0xFFFFFFFF);
	x4_hi = (uint32_t) ((x4 >> 32) & 0xFFFFFFFF);
	x4_lo = (uint32_t) (x4         & 0xFFFFFFFF);

	if (swap_endianess == 1) {
		x0_hi = SWAP_ENDIANESS(x0_hi);
		x0_lo = SWAP_ENDIANESS(x0_lo);
		x1_hi = SWAP_ENDIANESS(x1_hi);
		x1_lo = SWAP_ENDIANESS(x1_lo);
		x2_hi = SWAP_ENDIANESS(x2_hi);
		x2_lo = SWAP_ENDIANESS(x2_lo);
		x3_hi = SWAP_ENDIANESS(x3_hi);
		x3_lo = SWAP_ENDIANESS(x3_lo);
		x4_hi = SWAP_ENDIANESS(x4_hi);
		x4_lo = SWAP_ENDIANESS(x4_lo);
	}

	// // writing back to same registers
	WRITE_REG(12, sext_xlen(x0_hi));
	WRITE_REG(13, sext_xlen(x0_lo));
	WRITE_REG(14, sext_xlen(x1_hi));
	WRITE_REG(15, sext_xlen(x1_lo));
	WRITE_REG(16, sext_xlen(x2_hi));
	WRITE_REG(17, sext_xlen(x2_lo));
	WRITE_REG(28, sext_xlen(x3_hi));
	WRITE_REG(29, sext_xlen(x3_lo));
	WRITE_REG(30, sext_xlen(x4_hi));
	WRITE_REG(31, sext_xlen(x4_lo));
} else {



}
