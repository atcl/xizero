#!/usr/sbin/dtrace -s

#pragma D option ustackframes=100

dtrace:::BEGIN
{
	printf("Hit Ctrl-C to exit.\n");
}

profile-997
/execname == "xizero"/
{
	@s[ustack()] = count();
}

tick-30s
{
	printa(@s);
	exit(0);
}

syscall:::entry
/execname == "xizero"/
{
	@t[probefunc] = count();
}

syscall::rexit:
/execname == "xizero"/
{
	printa(@s);
	printa(@t);
	exit(0);
}

