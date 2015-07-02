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
#ifndef __MONOLINGUAL_H__
#define __MONOLINGUAL_H__

#include "tools.h"
#include "biword.h"
#include "alignmentData.h"



using namespace std;
using namespace Tools;

class monolingualModel
{
    private:
      multimap< size_t, biWord*  > * ms;
      multimap< string, size_t  > * mapS;
//       multimap< size_t, multimap< size_t, double  >* > * distance;
      int nthreads;
      vector< vector<float> > * d_scores;
      int m_nbest;
    public:
      monolingualModel();
      monolingualModel(string FileNameMS);
      ~monolingualModel();
      multimap< size_t, biWord*  > * getMS();
      void subprocess(biWord* l_bi_word);
      vector<biWord> * recherche(string s);
      vector<biWord> * recherche(string s, int nbest);
      float crossCosine(string s, string t);
      void oneToOneAlignment(string src, string tgt);
      vector< alignmentData > oneToManyAlignment(string src, string tgt);

};


#endif
