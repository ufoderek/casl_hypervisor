OUTPUT_ARCH(arm)
OUTPUT_FORMAT(elf32-littlearm)
ENTRY(reset_handler)

MEMORY
{
	ram(WAXI): org = 0x00000000, l = 128M
}

SECTIONS
{
	. = 0x00000000;
	
	. = ALIGN(4);
	
	.text :
	{
		builds/meteor/vector.o(.text)
	}
	
	_text = .;
	.text :
	{
		_start_text = .;

		builds/meteor/boot.o(.start)
		builds/meteor/exception_handler.o
		*(.text)

		_end_text = .;
	}

	. = ALIGN(4);
	_rodata = .;
	.rodata :
	{
		_start_rodata = .;

		*(.rodata)

		_end_rodata = .;
	}

	. = ALIGN(4);
	_data = .;
	.data :
	{
		_start_data = .;

		*(.data)

		_end_data = .;
	}

	. = ALIGN(4);
	
	_bss = .;
	.bss :
	{
		_start_bss = .;

		*(.bss)

		_end_bss = .;
	}

	_length_text =  _end_text - _start_text;
	_length_rodata = _end_rodata - _start_rodata;
	_length_data = _end_data - _start_data;
	_length_bss = _end_bss - _start_bss;
	_length_program = _end_bss - _start_text;
}
