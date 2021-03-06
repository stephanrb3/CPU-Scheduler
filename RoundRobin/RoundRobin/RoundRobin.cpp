//////////////////////////////
// CSCI 330 Stephan Brown   //
// Operating Systems        //
// Round Robin CPU Scheduler//
//////////////////////////////


//Arrival Time : Time at which the process arrives in the ready queue.
//Process Service Time : Time at which process completes its execution.
//Burst Time : Time required by a process for CPU execution.
//Turn Around Time : Time Difference between completion time and arrival time.
//Turn Around Time = Completion Time - Arrival Timed
//Waiting Time(W.T) : Time Difference between turn around time and burst time.
//Waiting Time : Turn Around Time - Burst Time
//Clock	: timestamps all events for processes, such as creation time, completion time, etc.
//Process Creator :	creates processes at arrival time
//Time Quantum	time each process can spend in the CPU, before it is removed
//Context Switch	number of times a process is switched

// CPU utilization = total burst time / (total burst time + idle time)


#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "iostream"
#include <string>
#include <cstdlib>
#include <stdio.h>  
#include <stdlib.h>  
using namespace std;


double firstArrival;

//Save Process Information
class ProcessClass {
private:
	int id;
	double RemaingTime;
	double BurstTime;
	double ArrivalTime;

public:
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);
	int get_id();
	void ExecutTime(double _time);
	double get_BurstTime();
	double get_ArrivalTime();
	double get_RemaingTime();
	double contextSwitchCount = 0;
	double get_Switches();

};

//Save Scheduling Information
class RRScheduling {
private:
	double quantumT;
	int executionIndex;
	double **ptrExecutionProcess; 
public:
	RRScheduling() {};
	RRScheduling(int);
	RRScheduling(int _proccessMAXNumber, double _quantum);
	~RRScheduling();
	void setQuantum(double);
	void setScheduling(int _process_id, double _startTime, double _endTime, double _contextSwitches);
	double * get_ExecutionProcess(int);
	int get_executionIndex();
	double get_quantum();
};

const int MAX = 100; //max processes


const char READ_FILE_NAME[] = "Round-Robin_test.txt";
const char WRITE_FILE_NAME[] = "Round-Robin_Result.txt";
const int MAX_PROCESS_NUMBER = 100;

void ReadProcessInfo(const char *FileName, ProcessClass **, int *, const int, RRScheduling *);
void Scheduling(ProcessClass **proccesArrayAddress, int *, RRScheduling *Scheduling);
void WriteExecutionResult(RRScheduling *, const char *, const char *);
void showCharArray(char *, int);
void resetCharArray(char *, int);

ProcessClass *processArray[MAX_PROCESS_NUMBER]; 
int processArray_Index = 0;
RRScheduling *RRSch = new RRScheduling(MAX_PROCESS_NUMBER);


RRScheduling::RRScheduling(int _processMAXNumber) {
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
}

RRScheduling::RRScheduling(int _processMAXNumber, double _quantum) {
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
	quantumT = _quantum;
}

RRScheduling::~RRScheduling() {}

void RRScheduling::setQuantum(double _quantum) { quantumT = _quantum; }

void RRScheduling::setScheduling(int _process_id, double _startTime, double _endTime, double _contextSwitches) {
	ptrExecutionProcess[executionIndex][0] = _process_id;
	ptrExecutionProcess[executionIndex][1] = _startTime;
	ptrExecutionProcess[executionIndex][2] = _endTime;
	ptrExecutionProcess[executionIndex][3] = _contextSwitches;
	executionIndex++;
}

double * RRScheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
int RRScheduling::get_executionIndex() { return executionIndex; };
double RRScheduling::get_quantum() { return quantumT; };



ProcessClass::ProcessClass(int _id, double _BurstTime, double _ArrivalTime) {
	id = _id;
	BurstTime = _BurstTime;
	ArrivalTime = _ArrivalTime;
	RemaingTime = _BurstTime;
}

int ProcessClass::get_id() { return id; };
double ProcessClass::get_BurstTime() { return BurstTime; };
double ProcessClass::get_ArrivalTime() { return ArrivalTime; };
double ProcessClass::get_RemaingTime() { return RemaingTime; }
double ProcessClass::get_Switches(){ return contextSwitchCount;}

void ProcessClass::ExecutTime(double _time) {
	RemaingTime = RemaingTime - _time;
}

//Total Burst
double tBurst;

void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER, RRScheduling *_RRSch) {
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {
			fin.close();
			cout << "\nRead file opening failed.\n";
			system("pause");
			exit(1);
		}
		cout << "\nFile Opened" << endl;

		//Save Process Information
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		double quantum;

		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {

			if (fin.eof()) {
				break;
			}
			else {
				fin.getline(process_id, BUFFER_SIZE, ',');
				fin.getline(burst_time, BUFFER_SIZE, ',');
				fin.getline(arrival_time, BUFFER_SIZE);

				if (*_proceeArray_Index == 0) {
					firstArrival = atof(arrival_time);
				}

				tBurst += atof(burst_time);
			}


			 //Save process to processArray
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi(process_id), atof(burst_time), atof(arrival_time));

			//Check Save
			cout << "Process Id = " << _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = " << _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << ", Arrival Time = " << _processArray[*_proceeArray_Index]->get_ArrivalTime();
			cout << endl;

			//Reset char array
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);
			resetCharArray(arrival_time, BUFFER_SIZE);


			*_proceeArray_Index += 1;
		}

		fin.close();
		cout << "Enter Quantum: ";
		cin >> quantum;
		_RRSch->setQuantum(quantum);
		cout << "Quantum = " << _RRSch->get_quantum() << endl;
	}
	catch (exception e) {
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}

}

double Throughput(int ProcessCount, double timeTotal) { 
	double throughput = ProcessCount / timeTotal;
	return throughput;
}

void Scheduling(ProcessClass **_processArray, int*_processArray_Index, RRScheduling *_RRSch) {

	double currentTime = firstArrival;
	int NotFinishProcessNumber = *_processArray_Index; 
	double tmpTime = 0;

	while (NotFinishProcessNumber>0) {
		for (int i = 0; i<*_processArray_Index; i++) {
			if (_processArray[i]->get_RemaingTime() > 0) {
				if (_processArray[i]->get_RemaingTime() > _RRSch->get_quantum()) {

					_RRSch->setScheduling(_processArray[i]->get_id(), currentTime, currentTime + _RRSch->get_quantum(), _processArray[i]->get_Switches());
					_processArray[i]->ExecutTime(_RRSch->get_quantum());
					currentTime += _RRSch->get_quantum();
					_processArray[i]->contextSwitchCount++;
				}
				else {
					_RRSch->setScheduling(_processArray[i]->get_id(), currentTime, currentTime + _processArray[i]->get_RemaingTime(), _processArray[i]->get_Switches());
					currentTime += _processArray[i]->get_RemaingTime();
					_processArray[i]->ExecutTime(_processArray[i]->get_RemaingTime());
					NotFinishProcessNumber -= 1;
				}
			}
		}
	}
}

double CPUutilization(double _idles) {
	//total burst time / (total burst time + idle time)

	//A context switch is the idle time so I'm representing idle time just a the number of context switches
	double CPU = tBurst/ (tBurst + _idles) * 100;
	return CPU;
}

void WriteExecutionResult(RRScheduling *_RRSch, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	double serviceTime;

	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}
	fout << endl;

	for (int i = 0; i < _RRSch->get_executionIndex(); i++) {
		cout << "pId = " << _RRSch->get_ExecutionProcess(i)[0];
		cout << ", start = " << _RRSch->get_ExecutionProcess(i)[1];
		cout << ", end = " << _RRSch->get_ExecutionProcess(i)[2];

		serviceTime = _RRSch->get_ExecutionProcess(i)[2] - _RRSch->get_ExecutionProcess(i)[1];

		cout << ", Process Service Time = " << serviceTime << "    ";
		cout << ", Context Switches : " << _RRSch->get_ExecutionProcess(i)[3] << "  ";
		cout << endl;

		fout << _RRSch->get_ExecutionProcess(i)[0];
		fout << " " << _RRSch->get_ExecutionProcess(i)[1];
		fout << " ~ " << _RRSch->get_ExecutionProcess(i)[2];
		fout << endl;

		if (i == _RRSch->get_executionIndex() - 1) {
			// last iteration 
			double endTime = _RRSch->get_ExecutionProcess(i)[2];
			cout << "Throughput: "<< Throughput(i, endTime) << endl;
			cout << "CPU Utilization: " << CPUutilization(_RRSch->get_ExecutionProcess(i)[3]) << "%" <<endl;
		}
	}

	fin.close();
	fout.close();
}


void showCharArray(char *_showchar, int _charsize) {

	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}

}


void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}
}


int main() {
	
	cout << "Start Round Robin" << endl;

	cout << "\n1. Read File" << endl;
	cout << "Accept File : " << READ_FILE_NAME << endl;
	ReadProcessInfo(READ_FILE_NAME, processArray, &processArray_Index, MAX_PROCESS_NUMBER, RRSch);

	cout << "\n2. Round Robin Scheduling Occuring" << endl;
	Scheduling(processArray, &processArray_Index, RRSch);

	cout << "\n3. Write File" << endl;
	WriteExecutionResult(RRSch, READ_FILE_NAME, WRITE_FILE_NAME);
	cout << "\nResult File Name = " << WRITE_FILE_NAME << endl;
}



