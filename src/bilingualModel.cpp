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

#include "bilingualModel.h"
#include <iostream>
#include <fstream>
#include <omp.h>

// #include <thread>
// #include <boost/lockfree/queue.hpp>

using namespace std;
using namespace Tools;

bilingualModel::bilingualModel()
{
      ms = new multimap< size_t, biWord* >();
      mt = new multimap< size_t, biWord* >();
      nthreads = 4;
}
multimap< size_t, biWord* >* bilingualModel::getMS()
{
    return ms;
}
multimap< size_t, biWord* >* bilingualModel::getMT()
{
    return mt;
}
bilingualModel::~bilingualModel()
{
    delete(ms);
    delete(mt);
}
bilingualModel::bilingualModel(string FileNameMS, string FileNameMT)
{
    ms = new multimap< size_t, biWord* >();
    mt = new multimap< size_t, biWord* >();
    nthreads = 4;
    ifstream fichierMS; 
    fichierMS.open( FileNameMS.c_str());
    ifstream fichierMT; 
    fichierMT.open( FileNameMT.c_str());
    string line="";
    vector<int> l_data;
    vector<string> l_data_str;
    int vocab_size_src=0;
    int vector_size=0;
    int vocab_size_tgt=0;
    biWord * l_biWord;
    int l_cpt=0;
    if (! fichierMS)
    {
	cerr << "Error while opening " << FileNameMS <<endl;
	exit(1);
    }
    if (! fichierMT)
    {
	cerr << "Error while opening " << FileNameMT <<endl;
	exit(1);
    }
    getline ( fichierMS, line );
    l_data=stringToVectorInt(line," ");
    if ((int)l_data.size() < 2)
    {
	cerr << "Error while extracting data: " << FileNameMS <<endl;
	exit(1);
    }
    vocab_size_src=l_data.at(0);
    vector_size=l_data.at(1);
    cerr << vocab_size_src << "\t" << vector_size << endl;
    pair<size_t, biWord*> * p;
    while (getline ( fichierMS, line ))
    {
	l_cpt++;
	l_data_str=stringToVector(line," ");
	l_biWord = new biWord(l_data_str.at(0), copyVectorDouble(l_data_str,1,vector_size + 1));
	p = new pair<size_t, biWord*>(l_biWord->getKey(),l_biWord);
	ms->insert((*p));
	if ((l_cpt % (vocab_size_src/100)) == 0) 
	{
	    cerr <<".";
	}
	delete(p);
    }
    cerr <<".OK!"<<endl;
    l_cpt=0;
    getline ( fichierMT, line );
    l_data=stringToVectorInt(line," ");
    if ((int)l_data.size() < 2)
    {
	cerr << "Error while extracting data: " << FileNameMT <<endl;
	exit(1);
    }
    vocab_size_tgt=l_data.at(0);
    vector_size=l_data.at(1);
    cerr << vocab_size_tgt << "\t" << vector_size << endl;
    while (getline ( fichierMT, line ))
    {
	l_cpt++;
	l_data_str=stringToVector(line," ");
	l_biWord = new biWord(l_data_str.at(0), copyVectorDouble(l_data_str,1,vector_size + 1));
	p = new pair<size_t, biWord*>(l_biWord->getKey(),l_biWord);
	mt->insert((*p));
	if ((l_cpt % (vocab_size_tgt/100)) == 0) 
	{
	    cerr <<".";
	}
	delete(p);
    }
    cerr <<".OK!"<<endl;
    distance = new multimap< size_t, multimap< size_t, double  >* > ();
    multimap< size_t, biWord* >::iterator l_iter_src;
    multimap< size_t, biWord* >::iterator l_iter_tgt;
    double l_score = 0.0;
    l_iter_src=ms->begin();
    l_cpt=0;
    # ifdef _OPENMP
      printf("Compiled by an OpenMP-compliant implementation.\n");
      omp_set_num_threads(10);
    # endif    
    #pragma omp parallel
    while (l_iter_src != ms->end())
    {
	l_cpt++;
	subprocess(l_iter_src);
	l_iter_src++;
	if ((l_cpt % (vocab_size_src/100)) == 0) 
	{
	    cerr <<".";
// 	    exit(0);
	}
    }
    cerr <<".OK!"<<endl;
    

}
void bilingualModel::subprocess(multimap< size_t, biWord* >::iterator l_iter_src)
{
	multimap< size_t, double  >* l_multimap = new multimap< size_t, double  >();
	multimap< size_t, biWord* >::iterator l_iter_tgt;
	l_iter_tgt=mt->begin();
	double l_score=0.0;
	while (l_iter_tgt != mt->end())
	{
	    l_score = Tools::cosine((*(l_iter_src)).second->getEmbeddings(),(*(l_iter_tgt)).second->getEmbeddings(),  (*l_iter_src).second->getMagnitude(), (*l_iter_tgt).second->getMagnitude());
	    pair<size_t, double> pt((*l_iter_tgt).first,l_score);
	    l_multimap->insert(pt);
	    l_iter_tgt++;
	}
	pair<size_t, multimap< size_t, double  >*> ps((*l_iter_src).first,l_multimap);
	distance->insert(ps);
	delete(l_multimap);
}

