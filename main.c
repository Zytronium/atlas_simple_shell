#include "main.h"

int main(int argc, char *argv[])
{
	int isInteractive = isatty(STDIN_FILENO);

	/* ------------------- On entry - one time execution ------------------- */
	(void) argc;
	/* if argc == 1, meaning no args given, meaning it's in interactive mode */
	if (isInteractive && stylePrints == 1)
		printf("%sWelcome to the %sGates Of Shell%s. Type \"exit\" to quit.\n\n",
			   CLR_YELLOW_BOLD, CLR_RED_BOLD, CLR_YELLOW_BOLD);
	/* --------------------------------------------------------------------- */

	shellLoop(isInteractive, argv); /* main shell loop */

	/* ------------------- On exit - one time execution ------------------- */
	if (isInteractive && stylePrints == 1)
		printf("%s\nThe %sGates Of Shell%s have closed. Goodbye.\n%s",
		   CLR_YELLOW_BOLD, CLR_RED_BOLD, CLR_YELLOW_BOLD, CLR_DEFAULT);

	return (EXIT_SUCCESS);
	/* -------------------------------------------------------------------- */
	/*
	 * note: 'On exit' code will probably never run unless something goes wrong.
	 * TODO: We should consider returning EXIT_FAILURE
	 *  or using break instead of exit(EXIT_SUCCESS)
	 */
}
