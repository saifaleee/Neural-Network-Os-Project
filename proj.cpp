#include <string>
#include "VectorToString.h"
#include "FileReading.h"
#include "MatrixMultiplication.h"


using namespace std;

int main(int argc, char* argv[])
{
	//Read neurons from previous layer
	char buffer[200] = {'0'};

	string layerno(argv[1]);
	string pipename = "allpipes/f_prog/forL" + layerno;

	while (access(pipename.c_str(), F_OK) == -1){sleep(1);}
	int piperead = open(pipename.c_str(), O_RDONLY);
	read(piperead, buffer, 200);
	close(piperead);
	unlink(pipename.c_str());


	vector<double> inputneurons = StringToVec(buffer);

	//--------    get weight matrix of this hidden layer
	vector<vector<double>> layer_weights;
	string layername = "Hidden layer " + layerno;
	layer_weights = ReadHiddenLayers(layername, "textfile.txt");


	//--------     matrix multiplcation using last read pipe
	vector<double> outputneurons = matrix_vector_multiply(layer_weights, inputneurons);

	cout << "-------------  " << atoi(argv[1])+1 <<" Layer ----------------\n";
	for (int i = 0; i < outputneurons.size(); i++)
		cout << outputneurons[i] << ", ";
	cout <<endl;

	string outbuf = VecToString(outputneurons);


	pid_t x = fork();
	//--------------  child
	if (x != 0)
	{
		//+1 is to check if its child should be normal or output  
		if (atoi(argv[1])+1 > atoi(argv[2]))
			execlp("./output", "./output", to_string(atoi(argv[1])+1).c_str(), to_string(atoi(argv[2])).c_str(), NULL);
		else
			execlp("./proj", "./proj", to_string(atoi(argv[1])+1).c_str(), to_string(atoi(argv[2])).c_str(), NULL);
	}
	
	//--------------  parent
	else
	{
		string pipename2 = "allpipes/f_prog/forL" + to_string(atoi(argv[1])+1);
		int nextpipecreate = mkfifo(pipename2.c_str(), 0666);
		int pipewrite = open(pipename2.c_str(), O_WRONLY);
		write(pipewrite, outbuf.c_str(), 200);
		close(pipewrite);

		//waitfor child/grandchildren to complete		
		//last child go to last layer == output
		//output generated at layer x
		//children ending
		//parent waiting for ex-child to write output in following pipe for back propogation
		
		char bufferbackw[200]= {'0'};

		string pipename3 = "allpipes/b_prog/forL" + layerno;
		while (access(pipename3.c_str(), F_OK) == -1){sleep(1);}
		int backpiperead = open(pipename3.c_str(), O_RDONLY);
		read(backpiperead, bufferbackw, 200);
		cout  <<endl << layerno <<" read: " <<bufferbackw <<" while going back.";
		close(backpiperead);
		unlink(pipename3.c_str());

		//--------write string to new pipe for previous layer - back propogation

		string pipename4 = "allpipes/b_prog/forL" + to_string(atoi(argv[1])-1);
		int nextpipebackwardcreate = mkfifo(pipename4.c_str(), 0666);
		int backpipewrite = open(pipename4.c_str(), O_WRONLY);
		write(backpipewrite, bufferbackw, 200);
		close(backpipewrite);
	}
	return 0;
}
