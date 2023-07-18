#include <string>
#include "VectorToString.h"
#include "FileReading.h"
#include "MatrixMultiplication.h"

using namespace std;

vector<double> FormulaFunc(double x) 
{
    double x1 = (x * x + x + 1) / 2;
    double x2 = (x * x - x) / 2;

    return {x1, x2};
}

int main()
{
	//---------  define number of hidden layers in network 
	string hiddenlayers = "5";

	//---------  read the input values from the textfile
	vector<double> input_data = ReadInputData("Input data", "textfile.txt");

	cout << "-------------  Inputs  ----------------\n";
	for (int i = 0; i < input_data.size(); i++)
		cout << input_data[i] << ", ";
	cout <<endl;

	//---------  read the input to 1st layer weights and multiply
	vector<vector<double>> inputlayer_weights;
	inputlayer_weights = ReadHiddenLayers("Input layer", "textfile.txt");

	vector<double> outputneurons = matrix_vector_multiply(inputlayer_weights, input_data);


	cout << "------------- 1st Layer ----------------\n";
	for (int i = 0; i < outputneurons.size(); i++)
		cout << outputneurons[i] << ", ";
	cout <<endl;

	string sendinput = VecToString(outputneurons);

	pid_t x = fork();
	//--------  create 1st hidden child/layer/process 
	if (x != 0)
		execlp("./proj", "./proj", "1", hiddenlayers.c_str(), NULL);

	else	//-----------------------------------------------------parent
	{
		//--------     write string to new pipe for next layer
		int nextpipecreate = mkfifo("allpipes/f_prog/forL1", 0666);
		int pipewrite = open("allpipes/f_prog/forL1", O_WRONLY);
		write(pipewrite, sendinput.c_str(), 200);
		close(pipewrite);

		//--------    wait for alllll child + grandchildren to complete
		//--------    not waiting for children instead waiting for pipe to be created so it can read

		while (access("allpipes/b_prog/forL0", F_OK) == -1){sleep(1);}
		int backpiperead = open("allpipes/b_prog/forL0", O_RDONLY);
		char bufferback[200] = {'0'};
		read(backpiperead, bufferback, 200);
		close(backpiperead);
		unlink("allpipes/b_prog/forL0");

		cout  <<endl << "Mid Output: " <<bufferback <<endl;

		//----- convert output val to new inputs using formula
		vector<double> newinputs = FormulaFunc(StringToVec(bufferback)[0]);

		cout << "------------- NEW Inputs ----------------\n";
		for (int i = 0; i < newinputs.size(); i++)
			cout << newinputs[i] << ", ";
		cout <<endl;

		vector<double> outputneurons2 = matrix_vector_multiply(inputlayer_weights, newinputs);
		cout << "------------- 1st Layer/Pipe ----------------\n";
		for (int i = 0; i < outputneurons2.size(); i++)
			cout << outputneurons2[i] << ", ";
		cout <<endl;

		string sendinput2 = VecToString(outputneurons2);

		pid_t x2 = fork();

		//--------------------------------------------------2nd child
		if (x2 != 0)
			execlp("./proj", "./proj", "1", hiddenlayers.c_str(), NULL);

		//--------------------------------------------------parent continues here
		else
		{
			//--------write string to new pipe for next layer
			int nextpipecreate2 = mkfifo("allpipes/f_prog/forL1", 0666);

			int piperead = open("allpipes/f_prog/forL1", O_WRONLY);
			write(piperead, sendinput2.c_str(), 200);
			close(piperead);

			char bufferbacklast[50];

			while (access("allpipes/b_prog/forL0", F_OK) == -1){sleep(1);}
			int backpiperead2 = open("allpipes/b_prog/forL0", O_RDONLY);
			read(backpiperead2, bufferbacklast, 50);
			close(backpiperead2);
			unlink("allpipes/b_prog/forL0");
			cout <<endl << "Final Output: " << bufferbacklast <<endl;
		}
	}
	return 0;
}