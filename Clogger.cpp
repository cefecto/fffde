/* Logger */

#include "Clogger.h"

void CLogger::LogThis(char * text)
{
	std::ofstream examplefile;
		examplefile.open ("testje.Log", std::ofstream::app);
		examplefile << text;
		examplefile << "\n";
		examplefile.close();
}


void CLogger::LogThisFloat(float f)
{
	char sz[101];
	sprintf(sz, "%G", f);
	std::ofstream examplefile;
		examplefile.open ("testje.Log", std::ofstream::app);
		examplefile << sz;
		examplefile << "\n";
		examplefile.close();
}
