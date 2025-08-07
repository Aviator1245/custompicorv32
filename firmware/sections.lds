MEMORY {
	/* The memory in the testbench is 128 KB in size.
	 * Adjust the length to match. */
	mem : ORIGIN = 0x00000000, LENGTH = 0x00020000
}

SECTIONS {
	.memory : {
		. = 0x000000;
		start*(.text);
		*(.text);
		*(*);
		end = .;
		. = ALIGN(4);
	} > mem
}
