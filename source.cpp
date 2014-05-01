#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include<sstream>
#include <exception>
#include <string>
using namespace std;

typedef string (*ParseRule)(string);

map<string,string> buildRegisterMap()
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
    result["null"]="F";

    return result;
}

map<string,string> registers=buildRegisterMap();

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


private:
    map<string,ParseRule> commands;

    string formatLine(string line)
    {
        for(int i = 0 ; i < line.length();i++)
        {
            if(line[i]==',')line[i]=' ';
        }
        cout<<line;
        return line;
    }

    //THIS FUNCTION SHOULD TAKE A HEX VALUE AND MAKE IT LITTLE ENDIAN
    string formatNumber(string number)
    {
        return number;
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
        string result = "20 ";
        stringstream temp;
        temp << line;
        string currentWord;

        //get "rrmovl;"
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

    static string _cmovle(string line)
    {
        return line;
    }

    static string _cmovl(string line)
    {
        return line;
    }

    static string _cmove(string line)
    {
        return line;
    }

    static string _cmovne(string line)
    {
        return line;
    }

    static string _cmovge(string line)
    {
        return line;
    }

    static string _cmovg(string line)
    {
        return line;
    }

    static string _irmovl(string line)
    {
        return line;
    }

    static string _rmmovl(string line)
    {
        return line;
    }

    static string _mrmovl(string line)
    {
        return line;
    }

    static string _addl(string line)
    {
        return line;
    }

    static string _subl(string line)
    {
        return line;
    }

    static string _andl(string line)
    {
        return line;
    }

    static string _xorl(string line)
    {
        return line;
    }

    static string _pushl(string line)
    {
        return line;
    }

    static string _popl(string line)
    {
        return line;
    }


};

int main()
{
    Parser parser;
    cout<<parser.processLine("rrmovl %eax,%ebx");
    return 0;
}
