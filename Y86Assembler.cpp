/*****************************************************\
******************THE Y86 ASSEMBLER********************
*******************Justin Sybrandt*********************
**********************Mike Boom************************
***********************5/3/2014************************
\*****************************************************/



#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include<sstream>
#include <exception>
#include <string>
using namespace std;

//parse rule function pointer
typedef string (*ParseRule)(string);

class Parser
{
public:
    Parser()
    {
        //define function pointers for parsing
        commands["halt"]=_halt;
        commands["nop"]=_nop;
        commands["rrmovl"]=_rrmovl;
        commands["cmovle"]=_cmovle;
        commands["cmovl"]=_cmovl;
        commands["cmove"]=_cmove;
        commands["cmovne"]=_cmovne;
        commands["cmovge"]=_cmovge;
        commands["cmovg"]=_cmovg;
        commands["irmovl"]=_irmovl;
        commands["rmmovl"]=_rmmovl;
        commands["mrmovl"]=_mrmovl;
        commands["addl"]=_addl;
        commands["subl"]=_subl;
        commands["andl"]=_andl;
        commands["xorl"]=_xorl;
        commands["pushl"]=_pushl;
        commands["popl"]=_popl;
    }

    //takes a file and sends each line to processLine
    void processFile(char* inputFile)
    {
        ifstream fin;
        string line;
        fin.open(inputFile);
        if(!fin)
        {
            throw exception();
        }

        while(getline(fin,line))
        {
            if(!isNullOrWhitespace(line))
                cout<<processLine(line)<<endl;
        }
        fin.close();
    }

private:

    bool isNullOrWhitespace(string x)
    {
            for(int i = 0; i<x.length(); i++)
                if(!isspace(x[i]))return false;
            return true;
    }

    //mapping of command words to command parsing functions
    map<string,ParseRule> commands;

    //mapping of register name to register number
    static map<string,string> registers;

    //instantiates register map
    static map<string,string> buildRegisterMap()
    {
        map<string,string> result;

        result["%eax"]="0";
        result["%ecx"]="1";
        result["%edx"]="2";
        result["%ebx"]="3";
        result["%esp"]="4";
        result["%ebp"]="5";
        result["%esi"]="6";
        result["%edi"]="7";

        return result;
    }

    //replaces ',' with ' ' and anything else we want to do
    string formatLine(string line)
    {
        for(int i = 0 ; i < line.length();i++)
        {
            if(line[i]==',')line[i]=' ';
        }
        return line;
    }

    //helps parse #(Reg)
	static void splitLEAL(string& D, string& rB)
	{
		for(int i=0; i < D.length(); i++)
		{
			if(D[i] == '(' || D[i] == ')')
				D[i] = ' ';
		}
		stringstream temp;
		temp << D;
		temp >> D;
		temp >> rB;
	}

    //reads a line and sends it to the appropriate parsing function
    string processLine(string line)
    {
        line = formatLine(line);

        string result;
        stringstream input;
        string command;

        input << line;
        input>>command;

        if(!commands.count(command))
        {
            throw exception();
        }
        else
        {
            return commands[command](line);
        }
    }

    //Takes a hex number and formats it for little endian
    static string formatNumber(string number)
    {
        string result;

        if(number[0]=='$') number = number.substr(1);
        if(number.substr(0,2)=="0x") number=number.substr(2);

        if(number.length()>8) throw exception();

        while(number.length()<8) number = "0"+number;

        for(int i = number.length()-2; i >= 0 ; i-=2)
        {
            result+=" ";
            result+= number[i];
            result+= number[i+1];
        }
        return result.substr(1);
    }

    //parsing function
    static string _halt(string line)
    {
        return "00";
    }

    //parsing function
    static string _nop(string line)
    {
        return "10";
    }

    //parsing function
    static string _rrmovl(string line)
    {
        return basic2RegisterCommand("20",line);
    }

    //parsing function
    static string _cmovle(string line)
    {
        return basic2RegisterCommand("21",line);
    }

    //parsing function
    static string _cmovl(string line)
    {
        return basic2RegisterCommand("22",line);
    }

    //parsing function
    static string _cmove(string line)
    {
        return basic2RegisterCommand("23",line);
    }

    //parsing function
    static string _cmovne(string line)
    {
        return basic2RegisterCommand("24",line);
    }

    //parsing function
    static string _cmovge(string line)
    {
        return basic2RegisterCommand("25",line);
    }

    //parsing function
    static string _cmovg(string line)
    {
        return basic2RegisterCommand("26",line);
    }

    //parsing function
    static string _irmovl(string line)
    {
		string result;
		stringstream temp;
		string rA;
		string V;

		temp << line;

		//read past the command word
		temp >> V;
		temp >> V;
		temp >> rA;

		result = "30 F" + registers[rA] +" "+ formatNumber(V);

		return result;
    }

    //parsing function
    static string _rmmovl(string line)
    {
        string result;
		stringstream temp;
		string rA;
		string D;
		string rB;

		temp << line;

		//read past command word
		temp >> rA;
		temp >> rA;
		temp >> D;

        //extract register and number separatly
		splitLEAL(D, rB);

		result = "40 " + registers[rA] + registers[rB] +" "+ formatNumber(D);

		return result;
    }

    //parsing function
    static string _mrmovl(string line)
    {
        string result;
		stringstream temp;
		string rA;
		string D;
		string rB;

		temp << line;

		//read past command word
		temp >> D;
		temp >> D;
		temp >> rA;

        //extract the D and rB
		splitLEAL(D, rB);

		result = "50 " + registers[rA] + registers[rB] +" "+ formatNumber(D);

		return result;
    }

    //parsing function
    static string _addl(string line)
    {
        return basic2RegisterCommand("60",line);
    }

    //parsing function
    static string _subl(string line)
    {
        return basic2RegisterCommand("61",line);
    }

    //parsing function
    static string _andl(string line)
    {
        return basic2RegisterCommand("62",line);
    }

    //parsing function
    static string _xorl(string line)
    {
        return basic2RegisterCommand("63",line);
    }

    //parsing function
    static string _pushl(string line)
    {
        string result = "A0 ";
        stringstream temp;
        string currentWord;

        temp << line;

        //move past "popl"
        temp>>currentWord;

        //get register
        temp>>currentWord;

        result += registers[currentWord] + "F";

        return result;
    }

    //parsing function
    static string _popl(string line)
    {
        string result = "B0 ";
        stringstream temp;
        string currentWord;

        temp << line;

        //move past "popl"
        temp>>currentWord;

        //get register
        temp>>currentWord;

        result += registers[currentWord] + "F";

        return result;
    }

    //This function parses all commands in the form of "Command Reg,Reg"
    static string basic2RegisterCommand(string opCode, string line)
    {
        string result = opCode + " ";
        stringstream temp;
        string currentWord;

        temp << line;

        //get "command;"
        temp>>currentWord;

        //get first register value
        temp>>currentWord;

        //add register on to result'
        result += registers[currentWord];

         //get second register value
        temp>>currentWord;

        //add 2nd register onto result
        result += registers[currentWord];

        return result;
    }

};

//properly sets the private static map
map<string,string> Parser::registers = Parser::buildRegisterMap();


int main(int argv, char** argc)
{
    //if the appropriate number of arguments is provided
    if(argv == 2)
    {
        Parser parser;
        parser.processFile(argc[1]);
    }
    return 0;
}
