/*********************************
 * tercpp: an open-source Translation Edit Rate (TER) scorer tool for Machine Translation.
 *
 * Copyright 2010-2013, Christophe Servan, LIUM, University of Le Mans, France
 * Copyright 2015, Christophe Servan, GETALP-LIG, University of Grenoble, France
 * Contact: christophe.servan@gmail.com
 *
 * The tercpp tool and library are free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by 
 * the Free Software Foundation, either version 3 of the licence, or
 * (at your option) any later version.
 *
 * This program and library are distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * **********************************/
#include "biword.h"

using namespace std;
using namespace Tools;
// namespace bilingualDistance
// {
biWord::biWord(string s, vector<double> v)
{
    key = Tools::hashValueBoost(s);
    token = new string(s);
    embbeddings = new vector<double>(v);
    magnitude = Tools::magnitude(embbeddings);
}
biWord::biWord(string s, vector<double> v, long int id)
{
    key = id;
    token = new string(s);
    embbeddings = new vector<double>(v);
    magnitude = Tools::magnitude(embbeddings);
}
double biWord::cosine(biWord* foreignBiWord)
{
    return Tools::cosine(getEmbeddings(), foreignBiWord->getEmbeddings(), getMagnitude(),foreignBiWord->getMagnitude() );
}
double biWord::cosine(vector< double >* foreignEmbeddings)
{
    return Tools::cosine(getEmbeddings(), foreignEmbeddings);
}
vector< double >* biWord::getEmbeddings()
{
    return embbeddings;
}
size_t biWord::getKey()
{
    return key;
}
double biWord::getMagnitude()
{
    return magnitude;
}
string* biWord::getToken()
{
    return token;
}
biWord::~biWord()
{
    delete(token);
    delete(embbeddings);
}

    
  
// }