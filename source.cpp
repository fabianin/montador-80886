#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include<sstream>
#include <exception>
#include <string>
using namespace std;

typedef string (*ParseRule)(string);



class Parser
{
public:
    Parser()
    {
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

    void processFile(string inputFile, string outputFile)
    {
        fstream fin(inputFile, ios::in);
        fstream fout(outputFile, ios::out);
        string line;
        if(!fin)
        {
            throw exception();
        }
        if(!fout)
        {
            throw exception();
        }
        while(fin)
        {
            getline(fin,line);
            fout<<processLine(line)<<endl;
        }
        fin.close();
        fout.close();
    }

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

        return commands[command](line);
    }

    //THIS FUNCTION SHOULD TAKE A HEX VALUE AND MAKE IT LITTLE ENDIAN
    string formatNumber(string number)
    {
        string result;

        number=number.substr(2);

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

private:
    map<string,ParseRule> commands;

    static map<string,string> registers;

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


    static string _halt(string line)
    {
        return "00";
    }

    static string _nop(string line)
    {
        return "10";
    }

    static string _rrmovl(string line)
    {
        return basic2RegisterCommand("20",line);
    }

    static string _cmovle(string line)
    {
        return basic2RegisterCommand("21",line);
    }

    static string _cmovl(string line)
    {
        return basic2RegisterCommand("22",line);
    }

    static string _cmove(string line)
    {
        return basic2RegisterCommand("23",line);
    }

    static string _cmovne(string line)
    {
        return basic2RegisterCommand("24",line);
    }

    static string _cmovge(string line)
    {
        return basic2RegisterCommand("25",line);
    }

    static string _cmovg(string line)
    {
        return basic2RegisterCommand("26",line);
    }

    static string _irmovl(string line)
    {
		string result;
		stringstream temp;
		string rA;
		string V;
		
		temp << line;
		temp >> V;
		temp >> V;
		temp >> rA;
		
		result = "30 F" + registers[rA] + formatNumber(V);
		
		return result;
    }

    static string _rmmovl(string line)
    {
        string result;
		stringstream temp;
		string rA;
		string D;
		string rB;
		
		temp << line;
		temp >> rA;
		temp >> rA;
		temp >> D;
		
		splitLEAL(D, rB);
		
		result = "40 " + registers[rA] + registers[rB] + formatNumber(D);
		
		return result;
    }

    static string _mrmovl(string line)
    {
        string result;
		stringstream temp;
		string rA;
		string D;
		string rB;
		
		temp << line;
		temp >> rA;
		temp >> rA;
		temp >> D;
		
		splitLEAL(D, rB);
		
		result = "50 " + registers[rA] + registers[rB] + formatNumber(D);
		
		return result;
    }

    static string _addl(string line)
    {
        return basic2RegisterCommand("60",line);
    }

    static string _subl(string line)
    {
        return basic2RegisterCommand("61",line);
    }

    static string _andl(string line)
    {
        return basic2RegisterCommand("62",line);
    }

    static string _xorl(string line)
    {
        return basic2RegisterCommand("63",line);
    }

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


int main()
{
    Parser parser;
    //cout<<parser.processLine("addl %eax,%ebx");
    cout<<parser.formatNumber("0x12345");
    return 0;
}
