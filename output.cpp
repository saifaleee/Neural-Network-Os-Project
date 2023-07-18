#include <string>
#include "VectorToString.h"
#include "FileReading.h"
#include "MatrixMultiplication.h"

using namespace std;

int main(int argc, char* argv[])
{
	char bufferout[200] = {'0'};
	
	string layerno(argv[1]);

	string pipename = "allpipes/f_prog/forL" + layerno;

	//----------pipe read from last hidden layer
	while (access(pipename.c_str(), F_OK) == -1){sleep(1);}
	int piperead = open(pipename.c_str(), O_RDONLY);
	read(piperead, bufferout, 200);
	close(piperead);
	unlink(pipename.c_str());


	vector<double> prevNeurons = StringToVec(bufferout);

	cout << "-------------  " << atoi(argv[1])+1 <<" Vector ----------------\n";
	for (int i = 0; i < prevNeurons.size(); i++)
		cout << prevNeurons[i] << ",";
	cout <<endl <<endl;
	
	//---------   get weights of output layer
	vector<double> output_layer;
	output_layer = ReadOutputLayer("Output layer", "textfile.txt");
	

	//calculate last output here
	//addition of pipe values from pipe read
	double sum = 0;
	for (int i = 0; i < output_layer.size(); i++)
		sum += prevNeurons[i] * output_layer[i];

	cout <<"Output: " <<sum <<endl <<endl;

	//--------write output to new pipe for previous layer - back propogation starts
	string pipenameback = "allpipes/b_prog/forL" + to_string(atoi(argv[1])-1);
	int nextpipebackwardcreate = mkfifo(pipenameback.c_str(), 0666);
	int backpipewrite = open(pipenameback.c_str(), O_WRONLY);
	write(backpipewrite, to_string(sum).c_str(), 200);
	close(backpipewrite);

	return 0;
}