# curde_neuralnetworks
This is a very simplistic attempt of indulging in Neural Networks, for our Operating Systems Course Project. 

## NOTE
Before testing the project, make sure to extract the zip folder, since it will be required to store pipes and not let it malfunction. Make sure that you paste the allpipes folder alongside the other code files, and be careful that inside the allpipes folder are 2 folders named "f_prog" and "b_prog".

## Run Files
This code is written to be run on Linux, so be mindful of that too.
After making executable files of all 3 codes, "start", "proj", "output", run the start.exe file. To make changes on size of network, depth or the weights of the network, you would need to make multiple changes accordingly:
1. Change the txt file:-(Add or remove matrix / change values here)
2. Change depth and network width values in both header files and the start.cpp.

## Explaination
Do not take this project as an example for neural networking projects, it was a very far reaching approach to neural networks and is no where close to be a proper example.
This project was intended to make use of our knowledge in basic Operating System concepts like Process Creation and Execution, Pipe Communication, Back Propogation and Threads. The neural network information is provided in the text file, including the weight matrices, input vectors and depths. The start.cpp file reads the inputs, and creates a child process which runs proc.cpp. Then start.cpp sends the input vector to the proc using pipes, which are multiplied with the weight matrix of that level. Multiplication of matrices is simplified using the concept of threads. Proc.cpp continues this loop until the last child runs output.cpp. When Output.cpp calculates its final answer, all the previous processes retrieve that value, send it to their parent proc and end themselves. When Start.cpp recieves this 1st output, it calculates 2 new input values and repeats the process. This time however, the processes back propogate the final answer, which start.cpp prints and finally exits.
No communication is done directly between processes, and every vector is shared using pipes. Additionally, after the creation and usage of each pipe, they automatically delete too, which was my highlight of this project.

Happy testing!! :)
