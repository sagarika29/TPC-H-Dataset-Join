#include "JoinQuery.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <string>
//---------------------------------------------------------------------------
using namespace std;
using std::unordered_map;

std::string lPath,oPath,cPath;

std::unordered_map<size_t,std::string> cust_map ; 
std::unordered_map<size_t,size_t> ord_map ; 

uint64_t sum_machinery=0;
uint64_t count_machinery=0;
uint64_t sum_furniture=0;
uint64_t count_furniture=0;
uint64_t sum_household=0;
uint64_t count_household=0;
uint64_t sum_automobile=0;
uint64_t count_automobile=0;
uint64_t sum_building=0;
uint64_t count_building=0;

JoinQuery::JoinQuery(std::string lineitem, std::string order,
                     std::string customer)
{
    lPath = lineitem;
    oPath = order ;
    cPath = customer;

    //ensuring file is not read everytime the constructor is called
    if (cust_map.size()==0)
    {
        //Read all the files and store values in a map so that we don't read again
        initLinetable();
    }
}

size_t JoinQuery::initLinetable()
{
    std::ifstream relation(cPath);
    size_t cust_ID=0;
    size_t word_count = 0;
    for (std::string line; std::getline(relation, line,'|');) 
    {
        if(relation.eof())
        {
             break;
        }
        if(word_count == 0)
        {
			cust_ID=std::stoi(line);
        }
		else if(word_count == 6)
		{
			cust_map[cust_ID]=line;
		}
        else if(word_count == 7)
        {
            word_count = 0;
            continue;
        }
		word_count++;
    }	

    std::ifstream relation_o(oPath);
    size_t ord_ID=0;
    size_t word_count_o = 0;

    for (std::string line; std::getline(relation_o, line,'|');) 
    {
        if(relation_o.eof())
        {
            break;
        }
        if(word_count_o == 0)
        {
			ord_ID=std::stoi(line);
        }
		else if(word_count_o == 1)
		{
            ord_map[ord_ID]=std::stoi(line);
		}
        else if(word_count_o == 8)
        {
            word_count_o = 0;
            continue;
        }
		word_count_o++;
    }	

    std::ifstream relation_l(lPath);
    size_t ord_ID_l;
    size_t word_count_l = 0;

    for (std::string line; std::getline(relation_l, line,'|');) 
    {
        if(relation_l.eof())
        {
            break;
        }
        if(word_count_l == 0)
        {
            ord_ID_l = std::stoi(line);
        }
		else if (word_count_l == 4)
		{
            if (cust_map[ord_map[ord_ID_l]]=="MACHINERY")
            {
                sum_machinery+= std::stoi(line);
                count_machinery+=1;
            }
            else if (cust_map[ord_map[ord_ID_l]]=="FURNITURE")
            {
                sum_furniture+= std::stoi(line);
                count_furniture+=1;
            }
            else if (cust_map[ord_map[ord_ID_l]]=="AUTOMOBILE")
            {
                sum_automobile+= std::stoi(line);
                count_automobile+=1;
            }
            else if (cust_map[ord_map[ord_ID_l]]=="BUILDING")
            {
                sum_building+= std::stoi(line);
                count_building+=1;
            }
            else
            {
                sum_household+= std::stoi(line);
                count_household+=1;
            }
		}
        else if(word_count_l == 15)
        {
            word_count_l = 0;
            continue;
        }
		word_count_l++;
    }	
    return 1;
}
//---------------------------------------------------------------------------
size_t JoinQuery::avg(std::string segmentParam)
{
    if (segmentParam=="MACHINERY")
    {
        return (sum_machinery*100)/count_machinery;
    }
    else if (segmentParam=="FURNITURE")
    {
        return (sum_furniture*100)/count_furniture;
    }
    else if (segmentParam=="AUTOMOBILE")
    {
        return (sum_automobile*100)/count_automobile;
    }
    else if (segmentParam=="BUILDING")
    {
        return (sum_building*100)/count_building;
    }
    else
    {
        return (sum_household*100)/count_household;
    }   
}
//---------------------------------------------------------------------------
size_t JoinQuery::lineCount(std::string rel)
{
   std::ifstream relation(rel);
   assert(relation);  // make sure the provided string references a file
   size_t n = 0;
   for (std::string line; std::getline(relation, line);) n++;
   return n;
}
//---------------------------------------------------------------------------