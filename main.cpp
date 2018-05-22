/*
//#######################################################################
  Author:   Arthur J. Miller
  FileName: main.cpp
  Full project at: github.com/milleraj66/ECE585_TomasuloAlgorithm
  Date:
            Start:  03-01-2016
            Update: 03-08-2016
            Update: 03-09-2016
            Update:  03-10-2016
            Final:  03-11-2016

  Purpose:  ECE 585 Tomasulo Algorithm
            Driver file for Tomasulo algorithm
  TODO: Implement LW/SW
//#######################################################################
*/

#include <iostream>
#include <iomanip>          // Print Table Formatting
#include <vector>
#include <fstream>
#include "ReservationStation.h"
#include "Instruction.h"    // MIPS Style Instruction Class
#include "RegisterStatus.h"
#include "operaciones.h"

using namespace std;

//#######################################################################
//**** Define Architecture
// NUMBER OF RESERVATION STATIONS
const int Num_ADD_RS = 4;
const int Num_MULT_RS = 2;
const int Num_DIV_RS = 3;
// RESERVATION STATION LATENCY
const int ADD_Lat = 4;
const int MULT_Lat = 12;
const int DIV_Lat = 38;
// Datapath Latency
const int ISSUE_Lat = 1;
const int WRITEBACK_Lat = 1;
//**** Do not edit these constants
// Global Clock
int Clock = 0;
// used to check if INST == WRITEBACKS to end program
bool Done = true;
int Total_WRITEBACKS = 0;
// Counter for current instruction to issue
int currentInst_ISSUE = 0;
// Temporary fix for errors due to RS names being numbers
// -> errors with REG/RS/REGSTATUS == zero
const int ZERO_REG = 5000;
const int RegStatusEmpty = 1000;
const int OperandAvailable = 1001;
const int OperandInit = 1002;
//#######################################################################

//#######################################################################
// Driver Functions
int ISSUE(vector<Instruction>& Inst,
          vector<ReservationStation>& ResStat,
          vector<RegisterStatus>& RegStat,
          vector<int>& Register);
void EXECUTE(vector<Instruction>& Inst,
             vector<ReservationStation>& ResStat,
             vector<RegisterStatus>& RegStat,
             vector<int>& Register);
void WRITEBACK(vector<Instruction>& Inst,
               vector<ReservationStation>& ResStat,
               vector<RegisterStatus>& RegStat,
               vector<int>& Register);
// Helper functions
void printRegisterStatus(vector<RegisterStatus> );
void printReservationStations(vector<ReservationStation> );
void printRegisters(vector<int> );
void printInstructions(vector<Instruction> );
void printTimingTable(vector<Instruction> );
operaciones convert(const std::string& str);
//#######################################################################

//#######################################################################
// MAIN DRIVER
int main(int argc, char* argv[]){
    if (argc < 2){
      cout<<"Se debe proveer el nombre del archivo con las instrucciones \n";
      exit(1);
    }
    //**** START Define Architecture

    //Read instruction file and put it into a vector
    vector<Instruction> Inst;
    ifstream fp;
    fp.open(argv[1]);
    //Check if the file exists
    if (!fp){
      cout<<"Nombre del archivo de instrucciones no valido \n";
      exit(1);
    }
    int reg_dest;
    int reg_first_source;
    int reg_second_source;
    string operation;
    int i = 0;
    while(fp >> reg_dest >> reg_first_source >> reg_second_source >> operation){
      Instruction instruccion(reg_dest,reg_first_source,reg_second_source,convert(operation));
      Inst.push_back(instruccion);
      cout<<Inst[i].get_destination_register()<<"\n";
      i++;
    }
    //// Input reservation station architecture
    // DONT FORGET TO UPDATE ^
    // RESERVATION STATION NUMBERS
    //const int Num_ADD_RS = 4;
    //const int Num_MULT_RS = 2;
    //const int Num_DIV_RS = 3;
    ReservationStation
            ADD1(AddOp, OperandInit),
            ADD2(AddOp, OperandInit),
            ADD3(AddOp, OperandInit),
            ADD4(AddOp, OperandInit);
    ReservationStation
            MULT1(MultOp, OperandInit),
            MULT2(MultOp, OperandInit);
    ReservationStation
            DIV1(DivOp, OperandInit),
            DIV2(DivOp, OperandInit),
            DIV3(DivOp, OperandInit);
    // Pack reservation stations into vector
    vector<ReservationStation> ResStation = {ADD1,
                                             ADD2,
                                             ADD3,
                                             ADD4,
                                             MULT1,
                                             MULT2,
                                             DIV1,
                                             DIV2,
                                             DIV3};

    // TODO: could make this a vector rather than a class object
    // Initialize register status objects
    RegisterStatus
            F0(RegStatusEmpty),F1(RegStatusEmpty),
            F2(RegStatusEmpty),F3(RegStatusEmpty),
            F4(RegStatusEmpty), F5(RegStatusEmpty),
            F6(RegStatusEmpty),F7(RegStatusEmpty),
            F8(RegStatusEmpty),F9(RegStatusEmpty),
            F10(RegStatusEmpty),F11(RegStatusEmpty),
            F12(RegStatusEmpty);
    // Pack register status objects into vector
    vector<RegisterStatus> RegisterStatus =
            {F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12};

    // Initialize register file vector
    vector<int> Register = {ZERO_REG,1,2,3,4,5,6,7,8,9,10,11,12};
    //**** END Define Architecture

    cout << "INITIAL VALUES:" << endl;
    printInstructions(Inst);
    printReservationStations(ResStation);
    printRegisters(Register);
    printRegisterStatus(RegisterStatus);
    cout << endl;

    //**** START functional loop
    do{
        // Datapath
        Clock++; // system clock

        ISSUE(Inst,ResStation,RegisterStatus,Register);
		EXECUTE(Inst,ResStation,RegisterStatus,Register);
		WRITEBACK(Inst,ResStation,RegisterStatus,Register);

        // PRINT
        printRegisters(Register);
        printTimingTable(Inst);
        cout << endl;

        // Check if all reservation stations are empty -> program done
        // TODO: if LW/SW are added this will need to be udated
        Done = false;
        if(Total_WRITEBACKS == Inst.size())
            Done = true;
        cout << endl;
	}while(!Done);//**** End functional loop

    return 0;
}//**** END MAIN DRIVER
//#######################################################################

//#######################################################################
// Datapath FUNCTIONS
int ISSUE(vector<Instruction>& INST,
          vector<ReservationStation>& RESSTATION,
          vector<RegisterStatus>& REGSTATUS,
          vector<int>& REG){
    // Latency of 1 if issued
    //**** check if spot in given reservation station is available
    int r = 0;
    bool rsFree = false;
    // r is the current instruction to be issued's operation
    // code(add,sub,mult,div)
    // If all instructions have been issued then stop issueing
    // for rest of program
    if(currentInst_ISSUE >= INST.size())
            return 0;
    r = INST[currentInst_ISSUE].get_operation();
    // determine if there is an open RS of r type. if yes
    // -> r = that open spot.
    // Boundry's of given RS
    int RSAddStart = Num_ADD_RS-Num_ADD_RS;
    int RSAddEnd = Num_ADD_RS;
    int RSSubStart = Num_ADD_RS-Num_ADD_RS;
    int RSSubEnd = Num_ADD_RS;
    int RSMulStart = Num_ADD_RS;
    int RSMulEnd = Num_ADD_RS+Num_MULT_RS;
    int RSDivStart = Num_ADD_RS+Num_MULT_RS;
    int RSDivEnd = Num_ADD_RS+Num_MULT_RS+Num_DIV_RS;
    switch(r){
        case AddOp:
            for(int i=RSAddStart;i<RSAddEnd;i++){
                if(!RESSTATION[i].is_busy()){
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].set_operation(AddOp);
                    rsFree = true;
                    break;
                }
            }
            // if instruction is not issued because no
            // reservation stations are free exit ISSUE
            // Init is not necessary if instruction not issued
            if(!rsFree)
                return 1;
            break;
        case SubOp:
            for(int i=RSSubStart;i<RSSubEnd;i++){
                if(!RESSTATION[i].is_busy()){
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].set_operation(SubOp);
                    rsFree = true;
                    break;
                }
            }
            if(!rsFree)
                return 1;
            break;
        case MultOp:
            for(int i=RSMulStart;i<RSMulEnd;i++){
                if(!RESSTATION[i].is_busy()){
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].set_operation(MultOp);
                    rsFree = true;
                    break;
                }
            }
            if(!rsFree)
                return 1;
            break;
        case DivOp:
            for(int i=RSDivStart;i<RSDivEnd;i++){
                if(!RESSTATION[i].is_busy()){
                    r = i;
                    currentInst_ISSUE++;
                    RESSTATION[i].set_operation(DivOp);
                    rsFree = true;
                    break;
                }
            }
            if(!rsFree)
                return 1;
            break;
        default:
            break;
    }
    //**** Initialize characteristics of issued instruction
    // if operand rs is available -> set value of operand
    // (Vj) to given register value
    // else point operand to the reservation station (Qj)
    // that will give the operand value
    // NOTE: since currentInst was in incremented we must
    // do currentINST_ISSUE-1
    if(REGSTATUS[INST[currentInst_ISSUE-1].get_rs_register()].get_estacion() == RegStatusEmpty){
        RESSTATION[r].set_Vj(REG[INST[currentInst_ISSUE-1].get_rs_register()]);
        RESSTATION[r].set_estacion_Qj(OperandAvailable);
    }
    else{
        RESSTATION[r].set_estacion_Qj(REGSTATUS[INST[currentInst_ISSUE-1].get_rs_register()].get_estacion());
    }
    // if operand rt is available -> set value of
    // operand (Vk) to given register value
    // else point operand to the reservation station
    // (Qk) that will give the operand value
    if(REGSTATUS[INST[currentInst_ISSUE-1].get_rt_register()].get_estacion() == RegStatusEmpty){
        RESSTATION[r].set_Vk(REG[INST[currentInst_ISSUE-1].get_rt_register()]);
        RESSTATION[r].set_estacion_Qk(OperandAvailable);
    }
    else{
        RESSTATION[r].set_estacion_Qk(REGSTATUS[INST[currentInst_ISSUE-1].get_rt_register()].get_estacion());
    }
    // given reservation station is now busy
    // until write back stage is completed.
    RESSTATION[r].set_busy();
    RESSTATION[r].set_issue_latency(0);
    // set reservation station instuction
    // number == current instruction
    RESSTATION[r].set_instruction_number(currentInst_ISSUE-1);
    // set clock cycle for issue time
    INST[currentInst_ISSUE-1].set_issue_clock(Clock);
    // The register status Qi is set to the current
    // instructions reservation station location r
    REGSTATUS[INST[currentInst_ISSUE-1].get_destination_register()].set_estacion(r);
    return 2;
}//END ISSUE()
void EXECUTE(vector<Instruction>& INST,
             vector<ReservationStation>& RESSTATION,
             vector<RegisterStatus>& REGSTATUS,
             vector<int>& REG){
    // check each reservation station to see
    // if both operands are ready
    // The current reservation station is r
    for (int r=0;r<RESSTATION.size();r++){
        // if both operands are available then
        // execute given instructions operation
        // and set resultReady flag to true so that
        // result can be written back to CDB
        // first check if instruction has been issued
        if(RESSTATION[r].is_busy() == true){
            // second check if the ISSUE latency clock cycle has happened
            if(RESSTATION[r].get_issue_latency() >= ISSUE_Lat){
                // third check if both operands are available
                if(RESSTATION[r].get_estacion_Qj() == OperandAvailable &&
                        RESSTATION[r].get_estacion_Qk() == OperandAvailable){
                    // Set clock cycle when execution begins
                    if(INST[RESSTATION[r].get_instruction_number()].get_execeute_clock_begin() == 0)
                        INST[RESSTATION[r].get_instruction_number()].set_execute_clock_begin(Clock);
                    // when execution starts we must wait the given
                    // latency number of clock cycles before making result
                    // available to WriteBack
                    // Delay: Switch(INST.op)
                    //		case(add): 	clock += 4;
                    //		case(mult): 	clock += 12;
                    //		case(div):	clock += 38;
                    RESSTATION[r].increase_latency();
                    switch(RESSTATION[r].get_operation()){
                        case(AddOp):
                            if(RESSTATION[r].get_lat() == ADD_Lat){
                                RESSTATION[r].set_result(RESSTATION[r].get_Vj() + RESSTATION[r].get_Vk());
                                // Result is ready to be writenback
                                RESSTATION[r].set_result_ready(true);
                                RESSTATION[r].set_lat(0);
                                // Set clock cycle when execution ends
                                INST[RESSTATION[r].get_instruction_number()].set_execute_clock_end(Clock);
                                // reset ISSUE latency for RS
                                RESSTATION[r].set_issue_latency(0);
                            }
                        case(SubOp):
                            if(RESSTATION[r].get_lat() == ADD_Lat){
                                RESSTATION[r].set_result(RESSTATION[r].get_Vj() - RESSTATION[r].get_Vk());
                                RESSTATION[r].set_result_ready(true);
                                RESSTATION[r].set_lat(0);
                                // Set clock cycle when execution ends
                                INST[RESSTATION[r].get_instruction_number()].set_execute_clock_end(Clock);
                                // reset ISSUE latency for RS
                                RESSTATION[r].set_issue_latency(0);
                            }
                        case(MultOp):
                            if(RESSTATION[r].get_lat() == MULT_Lat){
                                RESSTATION[r].set_result(RESSTATION[r].get_Vj() * RESSTATION[r].get_Vk());
                                RESSTATION[r].set_result_ready(true);
                                RESSTATION[r].set_lat(0);
                                // Set clock cycle when execution ends
                                INST[RESSTATION[r].get_instruction_number()].set_execute_clock_end(Clock);
                                // reset ISSUE latency for RS
                                RESSTATION[r].set_issue_latency(0);
                            }
                        case(DivOp):
                            if(RESSTATION[r].get_lat() == DIV_Lat){
                                RESSTATION[r].set_result(RESSTATION[r].get_Vj() / RESSTATION[r].get_Vk());
                                RESSTATION[r].set_result_ready(true);
                                RESSTATION[r].set_lat(0);
                                // Set clock cycle when execution ends
                                INST[RESSTATION[r].get_instruction_number()].set_execute_clock_end(Clock);
                                // reset ISSUE latency for RS
                                RESSTATION[r].set_issue_latency(0);
                            }
                        default:
                            break;
                    }
                }
            }
            else // Execute is not ready until one cycle latency of ISSUE
                RESSTATION[r].increase_issue_latency();
        }

    }

}//END EXECUTE()
void WRITEBACK(vector<Instruction>& INST,
               vector<ReservationStation>& RESSTATION,
               vector<RegisterStatus>& REGSTATUS,
               vector<int>& REG){
    // Check each reservation station to see
    // if operational delay is done -> result is ready
    for(int r=0;r<RESSTATION.size();r++){
        // if result ready write back to CDB
        // -> Register,and reservation stations
        if(RESSTATION[r].is_result_ready()){
            // Before Writeback is available there
            // must be a 1 cycle WB delay
            if(RESSTATION[r].get_writeback_latency() == WRITEBACK_Lat){
                // set clock cycle when write back occured.
                // (Must add one because increment happens after loop)
                if(INST[RESSTATION[r].get_instruction_number()].get_writeback_clock() == 0)
                    INST[RESSTATION[r].get_instruction_number()].set_writeback_clock(Clock);
                // Check if any registers (via the registerStatus)
                // are waiting for current r result
                for(int x=0;x<REG.size();x++) {
                    // if RegisterStatus points to the given
                    // reservation station r set that register[x]
                    // equal to executed result
                    if (REGSTATUS[x].get_estacion() == r) {
                        // Write back to Registers
                        REG[x] = RESSTATION[r].get_result();
                        REGSTATUS[x].set_estacion(RegStatusEmpty);
                    }
                }
                // Check if any reservation stations are
                // waiting for current r result
                for(int y=0;y<RESSTATION.size();y++){
                    // check if any reservation stations are
                    // waiting for the given result as an operand
                    // Write back to reservation stations
                    // Given RS is not longer waiting for this
                    // operand value
                    if(RESSTATION[y].get_estacion_Qj()==r){
                        RESSTATION[y].set_Vj(RESSTATION[r].get_result());
                        RESSTATION[y].set_estacion_Qj(OperandAvailable);
                    }
                    if(RESSTATION[y].get_estacion_Qk()==r){
                        RESSTATION[y].set_Vk(RESSTATION[r].get_result());
                        RESSTATION[y].set_estacion_Qk(OperandAvailable);
                    }
                }
                // The given reservation station can
                // now be used again
                // Reset RS paramaters
                RESSTATION[r].set_result_ready(false);
                RESSTATION[r].set_free();
                RESSTATION[r].set_estacion_Qj(OperandInit);
                RESSTATION[r].set_estacion_Qk(OperandInit);
                RESSTATION[r].set_Vj(0);
                RESSTATION[r].set_Vk(0);
                RESSTATION[r].set_writeback_latency(0);
                Total_WRITEBACKS++;
            }
            else
                RESSTATION[r].increase_writeback_latency();
        }
    }

}//END WRITEBACK()
//#######################################################################

//#######################################################################
// Helper Functions
void printRegisterStatus(vector<RegisterStatus> RegisterStatusVector){
    cout << "Register Status: " << endl;
    for(int i=0; i<RegisterStatusVector.size(); i++)
        cout << RegisterStatusVector[i].get_estacion() << ' ';
    cout << endl;
}
void printReservationStations(vector<ReservationStation> RSV){
    for(int i=0; i<RSV.size(); i++)
        cout << "RS #: " << i << "  Busy: " << RSV[i].is_busy() << "  op: "<<
                RSV[i].get_operation() << "  Vj: " << RSV[i].get_Vj() << "  Vk: " <<
                RSV[i].get_Vk() << "  Qj: " << RSV[i].get_estacion_Qj() << "  Qk: " <<
                RSV[i].get_estacion_Qk() << endl;
}
void printRegisters(vector<int> RegistersVector){
    cout << "Register Content:" << endl;
    for(int i=0; i<RegistersVector.size(); i++)
        cout << RegistersVector[i] << ' ';
    cout << endl;
}
void printInstructions(vector<Instruction> IV){
    for(int i=0; i<IV.size(); i++)
        cout << "Instruction #: " << i << "  Operation: " <<
                IV[i].get_operation() << "  " <<
                IV[i].get_destination_register() << " <- " << IV[i].get_rs_register() << " op " <<
                IV[i].get_rt_register() << endl;
}
void printTimingTable(vector<Instruction> INST){
    char separator    = ' ';
    const int width     = 10;
    char lineSeperator = '-';
    const int lineWidth = 30;

    // Define column labels
    cout << left << setw(width) << setfill(separator) << "Inst";
    cout << left << setw(width) << setfill(separator) << "Issue";
    cout << left << setw(width) << setfill(separator) << "Execute";
    cout << left << setw(width) << setfill(separator) << "WB";
    cout << left << setw(width) << setfill(separator) << "SystemClock"
    << endl;
    cout << right << setw(width*5) << setfill(separator) << Clock;
    cout << endl;
    cout << left << setw(lineWidth) << setfill(lineSeperator);
    cout << endl;
    // Define Row Labels and values
    for(int i=0;i<INST.size();i++){
        cout << left  << setw(width) << setfill(separator) << i;
        cout << left << setw(width) << setfill(separator) << INST[i].get_issue_clock();
        cout << INST[i].get_execeute_clock_begin() <<  "-";
        cout << left << setw(width) << setfill(separator)  << INST[i].get_execute_clock_end();
        cout << left << setw(width) << setfill(separator) << INST[i].get_writeback_clock();
        cout << endl;
    }

}

operaciones convert(const std::string& str)
{
    if(str == "AddOp") return AddOp;
    else if(str == "SubOp") return SubOp;
    else if(str == "MultOp") return MultOp;
    else if(str == "DivOp") return DivOp;
    else return AddOp;
}
//#######################################################################
