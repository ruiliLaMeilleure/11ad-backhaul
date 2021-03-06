/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 NICOLO' FACCHI
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nicolo' Facchi <nicolo.facchi@gmail.com>
 Rui Li <lirui628@gmail.com>
 */


#include "dmg-almighty-controller.h"
#include "ns3/log.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mobility-model.h"
#include "ns3/wifi-mac-queue.h"
#include "dmg-destination-fixed-wifi-manager.h"
#include "dmg-wifi-mac.h"
#include "yans-wifi-phy.h"
#include "yans-wifi-channel.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/pointer.h"
#include "edca-txop-n.h"
#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("DmgAlmightyController");

namespace ns3 {

DmgAlmightyController::DmgAlmightyController ()
{
	m_meshNodes = 0;
	// Configure some default values. Not all these values are used in the current
	// implementation.
	m_antennaSectorsN = 4; // by default 4 sectors (90 degrees)
	m_energyDetectionThreshold = -96.0;
	m_ccaMode1Threshold = -99;
	m_txGain = 1;
	m_rxGain = 1;
	m_txPowerLevels = 1;
	m_txPowerEnd = 16.0206;
	m_txPowerStart = 16.0206;
	m_rxNoiseFigure = 7;

	m_biDuration = 1024000 * 100;
	m_biOverhaedFraction = 0;

	m_beamSwitchOverheadNs = 0;
    m_sim_interference = true;
}

DmgAlmightyController::~DmgAlmightyController ()
{
	delete m_meshNodes;
	m_meshNodes = 0;
}

	void
DmgAlmightyController::SetMeshNodes (NodeContainer *meshnodes)
{
	delete m_meshNodes;
	m_meshNodes = 0;
	m_meshNodes = new NodeContainer(*meshnodes);
}

	NodeContainer *
DmgAlmightyController::GetMeshNodes (void)
{
	return m_meshNodes;
}

	void
DmgAlmightyController::SetFlowsPath(std::vector < std::vector <uint32_t> > path)
{
	m_flowsPath = path;
}

	void
DmgAlmightyController::SetAntennaSectorsN (double sectorsN)
{
	// Not used in current implementation.
	m_antennaSectorsN = sectorsN;
	// TODO: compute m_antennaBeamwidth and m_antennaGainTxRx
}

	double
DmgAlmightyController::GetAntennaSectorsN (void)
{
	return m_antennaSectorsN;
}

	void
DmgAlmightyController::SetAntennaBeamwidth (double beamwidth)
{
	// Not used in current implementation.
	m_antennaBeamwidth = beamwidth;
	// TODO: compute m_antennaSectorsN and m_antennaGainTxRx
}

	double
DmgAlmightyController::GetAntennaBeamwidth (void)
{
	return m_antennaBeamwidth;
}

	void
DmgAlmightyController::SetAntennaGainTxRx (double gain)
{
	// Not used in current implementation.
	m_antennaGainTxRx = gain;
	// TODO: compute m_antennaBeamwidth and m_antennaBeamwidth
}

	double
DmgAlmightyController::GetAntennaGainTxRx (void)
{
	// Not used in current implementation.
	return m_antennaGainTxRx;
}

	void
DmgAlmightyController::SetEnergyDetectionThreshold (double detTh)
{
	m_energyDetectionThreshold = detTh;
}

	double
DmgAlmightyController::GetEnergyDetectionThreshold (void)
{
	// Not used in current implementation.
	return m_energyDetectionThreshold;
}

	void
DmgAlmightyController::SetCcaMode1Threshold (double cca)
{
	// Not used in current implementation.
	m_ccaMode1Threshold = cca;
}

	double
DmgAlmightyController::GetCcaMode1Threshold (void)
{
	return m_ccaMode1Threshold;
}

	void
DmgAlmightyController::SetTxGain (double gain)
{
	// Not used in current implementation.
	m_txGain = gain;
}

	double
DmgAlmightyController::GetTxGain (void)
{
	return m_txGain;
}

	void
DmgAlmightyController::SetRxGain (double gain)
{
	// Not used in current implementation.
	m_rxGain = gain;
}

	double
DmgAlmightyController::GetRxGain (void)
{
	return m_rxGain;
}

	void
DmgAlmightyController::SetTxPowerLevels (double txLevels)
{
	// Not used in current implementation.
	m_txPowerLevels = txLevels;
}

	double
DmgAlmightyController::GetTxPowerLevels (void)
{
	return m_txPowerLevels;
}

	void
DmgAlmightyController::SetTxPowerEnd (double txPowerEnd)
{
	// Not used in current implementation.
	m_txPowerEnd = txPowerEnd;
}

	double
DmgAlmightyController::GetTxPowerEnd (void)
{
	return m_txPowerEnd;
}

	void
DmgAlmightyController::SetTxPowerStart (double txPowerStart)
{
	// Not used in current implementation.
	m_txPowerStart = txPowerStart;
}

	double
DmgAlmightyController::GetTxPowerStart (void)
{
	return m_txPowerStart;
}

	void
DmgAlmightyController::SetRxNoiseFigure (double noiseFigure)
{
	// Not used in current implementation.
	m_rxNoiseFigure = noiseFigure;
}

	double
DmgAlmightyController::GetRxNoiseFigure (void)
{
	return m_rxNoiseFigure;
}

	void
DmgAlmightyController::SetBiDuration (uint64_t biDur)
{
	m_biDuration = biDur;
}

	uint64_t
DmgAlmightyController::GetBiDuration (void)
{
	return m_biDuration;
}

	void
DmgAlmightyController::SetBiOverheadFraction (double biOverhead)
{
	m_biOverhaedFraction = biOverhead;
}

	double
DmgAlmightyController::GetBiOverheadFraction (void)
{
	return m_biOverhaedFraction;
}

	void
DmgAlmightyController::SetBeamSwitchOverhead (uint64_t beamSwitchOverhead)
{
	m_beamSwitchOverheadNs = beamSwitchOverhead;
}

	uint64_t
DmgAlmightyController::GetBeamSwitchOverhead (void)
{
	return m_beamSwitchOverheadNs;
}

	void
DmgAlmightyController::SetNumSchedulePerBi(uint32_t numSchedulePerBi)
{
	m_numSchedulePerBi = numSchedulePerBi;
}

	void
DmgAlmightyController::SetAckTimeFrac(double ackTimeFrac)
{
	m_ackTimeFrac = ackTimeFrac;
}

	void
DmgAlmightyController::SetServicePeriodAllocation (std::vector < std::vector <double> >  spAlloc)
{
	m_spAlloc = spAlloc;
}

	std::vector < std::vector <double> >
DmgAlmightyController::GetServicePeriodAllocation (void)
{
	return m_spAlloc;
}

	void
DmgAlmightyController::ConfigureWritePath (std::string w2str)
{
        m_writePath = w2str; 
}
    
void
DmgAlmightyController::SetSimInterference (bool sim_intf)
{
    m_sim_interference = sim_intf;
}
    
std::vector <uint32_t>
DmgAlmightyController::GetSegIndicesInClique(std::vector <uint32_t> link, std::vector < std::vector < uint32_t > > flowSegs)
{
    uint32_t link_from = link[0];
    uint32_t link_to = link[1];
    
    std::vector <uint32_t> seg_ids;
        for (uint32_t i= 0; i < flowSegs.size(); i++){
            if ((link_from == flowSegs[i][0] && link_to == flowSegs[i][1]) || (link_from == flowSegs[i][1] && link_to == flowSegs[i][0])){
                seg_ids.push_back(i);
//                NS_LOG_INFO("seg id "<< i << "from "<< link_from << "to "<< link_to << "flowSegs[i][0] "<< flowSegs[i][0] <<" flowSegs[i][1] "<< flowSegs[i][1] );
            }
        
        }
    return seg_ids;
}
/*Read from flowsPath and build a topology knowledge of Cliques and link list 
 * that will help in configuring wifi manager and preparing for progressive filling
 */
	void
DmgAlmightyController::ConfigureCliques (void)
{
	//prepare m_linkList (link noted in stations denoting the link *towards Sta 0 (gateway)*)
	std::vector <uint32_t> link (2); 
	for (uint32_t i = 0; i< m_flowsPath.front().size() - 1; i++){

		link[0] = (m_flowsPath.front().at(i) > m_flowsPath.front().at(i + 1))?(m_flowsPath.front().at(i)):(m_flowsPath.front().at(i + 1));
		link[1] = (m_flowsPath.front().at(i) < m_flowsPath.front().at(i + 1))?(m_flowsPath.front().at(i)):(m_flowsPath.front().at(i + 1));

		m_linkList.push_back(link);
	}

	for(uint32_t flowIdx = 1; flowIdx < m_flowsPath.size(); flowIdx++){
		for (uint32_t i = 0; i < m_flowsPath.at(flowIdx).size() - 1; i++){

			bool ifMatch = false;
			uint32_t linkIdx = 0;

			while(!ifMatch && linkIdx < m_linkList.size()){

				if ( ((m_flowsPath.at(flowIdx).at(i) == m_linkList.at(linkIdx).at(0)) && (m_flowsPath.at(flowIdx).at(i + 1) == m_linkList.at(linkIdx).at(1))) || ((m_flowsPath.at(flowIdx).at(i) == m_linkList.at(linkIdx).at(1)) && (m_flowsPath.at(flowIdx).at(i + 1) == m_linkList.at(linkIdx).at(0))) ){
					ifMatch = true;
					break;
				}
				linkIdx++;
			}
			if(!ifMatch){
				link[0] = (m_flowsPath[flowIdx][i] > m_flowsPath[flowIdx][i + 1])? (m_flowsPath[flowIdx][i]):(m_flowsPath[flowIdx][i + 1]);
				link[1] = (m_flowsPath[flowIdx][i] < m_flowsPath[flowIdx][i + 1])? (m_flowsPath[flowIdx][i]):(m_flowsPath[flowIdx][i + 1]);
				m_linkList.push_back(link);
			}
		}
	}

	for (uint32_t linkIdx = 0; linkIdx < m_linkList.size() - 1; linkIdx++){
		for (uint32_t sortingIdx = linkIdx + 1; sortingIdx < m_linkList.size(); sortingIdx++) {
			if (m_linkList[linkIdx] > m_linkList[sortingIdx]) {
				std::swap(m_linkList.at(linkIdx).at(0), m_linkList.at(sortingIdx).at(0));
				std::swap(m_linkList.at(linkIdx).at(1), m_linkList.at(sortingIdx).at(1));
			}
		}
	}

	//Print linkList for debugging
	for ( uint32_t linkIdx = 0; linkIdx < m_linkList.size(); linkIdx++){
		//NS_LOG_INFO("Link " << linkIdx << " from " << m_linkList[linkIdx][0] << " to " <<m_linkList[linkIdx][1] );
	}


	//Configure neighbourhood Info m_neighbourNodes //------needed?
	for( uint32_t nodeIdx = 0; nodeIdx < m_meshNodes->GetN(); nodeIdx++)
	{
		uint32_t neighIdx = 0;
		std::vector<uint32_t> neighbours;
		for ( uint32_t i = 0; i < m_linkList.size(); i++)
		{
			if ( nodeIdx == m_linkList[i][0] )
			{
				neighIdx++;
				neighbours.push_back(m_linkList[i][1]);
			}
			if ( nodeIdx == m_linkList[i][1])
			{
				neighIdx++;
				neighbours.push_back(m_linkList[i][0]);
			}
		}
		m_neighbourNodes.push_back (neighbours);

		//Build clique if the current node is a conflict node, i.e. has more than 1 neighbour, last element is the node itself
		if ( neighIdx > 1 )
		{
			neighbours.push_back(nodeIdx);

			cliqueStruct c;
			c.staMem= neighbours;
			c.staMemN =neighbours.size();

			cliqueS.push_back(c);
		}
	}

	//Printing cliqueS.staMemN and staMem for debugging.
	for ( uint32_t cIdx = 0; cIdx< cliqueS.size(); cIdx++){
		//NS_LOG_INFO("Clique "<< cIdx <<" has "<< cliqueS[cIdx].staMemN <<" stations: ");
		for (uint32_t i =0; i < cliqueS[cIdx].staMemN; i++){
			//NS_LOG_INFO(cliqueS[cIdx].staMem[i]);
		}
	}

	m_nextHops.push_back(0);
	for ( uint32_t i = 0; i < m_linkList.size(); i++){
		m_nextHops.push_back(m_linkList[i][1]);
	}

	for ( uint32_t i = 0; i < m_nextHops.size(); i++){
		//NS_LOG_INFO("next hop "<< i <<" is "<< m_nextHops.at(i));
	}

	for ( uint32_t flowIdx=0; flowIdx < m_flowsPath.size(); flowIdx++){
		for ( uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++){
			//build clique.flows and clique.flowSegs using flowPath
			for (uint32_t i = 0; i < m_flowsPath[flowIdx].size() - 1; i++){
				//if the clique has both stations of the current link
				if((std::find(cliqueS[cIdx].staMem.begin(), cliqueS[cIdx].staMem.end(), m_flowsPath[flowIdx][i]) != cliqueS[cIdx].staMem.end()) && (std::find(cliqueS[cIdx].staMem.begin(), cliqueS[cIdx].staMem.end(), m_flowsPath[flowIdx][i+1]) != cliqueS[cIdx].staMem.end())){
					cliqueS[cIdx].flows.push_back(flowIdx);

					std::vector <uint32_t> link;
					link.push_back(m_flowsPath[flowIdx][i]);
					link.push_back(m_flowsPath[flowIdx][i+1]);
					cliqueS[cIdx].flowSegs.push_back(link);

					cliqueS[cIdx].phyRate.push_back(0);
					cliqueS[cIdx].timeAlloc.push_back(0);//Initiate time for each flow segment. Will be configured in progressive filling
				}
			}
		}
	}

	//Print path of flow for debugging
/*	for ( uint32_t flowIdx=0; flowIdx < m_flowsPath.size(); flowIdx++){

		NS_LOG_INFO("Path of Flow "<< flowIdx<< ":(" << m_flowsPath[flowIdx].size()<<"STAs");
		for(uint32_t i = 0;i < m_flowsPath[flowIdx].size();i++ ){
			NS_LOG_INFO(" "<< m_flowsPath[flowIdx][i]);
		}
	}
	//Print clique.flows and clique.flowSegs
	for ( uint32_t cIdx = 0; cIdx< cliqueS.size();cIdx++){
		NS_LOG_INFO("Clique "<< cIdx <<" has ");

		for (uint32_t i =0; i < cliqueS[cIdx].flows.size(); i++){
			NS_LOG_INFO("flow "<< cliqueS[cIdx].flows[i] << ". Seg between ");
			for (uint32_t j = 0; j< cliqueS[cIdx].flowSegs[i].size();j++)	{
				NS_LOG_INFO( cliqueS[cIdx].flowSegs[i][j]<<" ");
			}
		}
	}*/
}


	void
DmgAlmightyController::SetGw  (uint32_t node)
{
	m_gw = node;
}
	uint32_t
DmgAlmightyController::GetGw  (void)
{
	return m_gw;
}

void
DmgAlmightyController::ConfigureHierarchy (void)
{
	NS_LOG_FUNCTION(this);
	std::vector < std::vector <uint32_t> > hierarchy;
	std::vector <uint32_t> conflictNodes;

    m_master.clear();
    m_masterClique.clear();
    m_schedulingOrder.clear();

	for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++) 
	{
		uint32_t cfl = cliqueS[cIdx].staMem.back();
		conflictNodes.push_back(cfl);
		if (cfl ==GetGw())
		{
			m_master.insert(std::pair<int32_t, int32_t>(cfl, 0));
			m_masterClique.insert(std::pair<int32_t, int32_t>(cfl, 0));
			m_schedulingOrder.push_back(cfl);
		}
		else{
			m_master.insert(std::pair<int32_t, int32_t>(cfl, -1));
			m_masterClique.insert(std::pair<int32_t, int32_t>(cfl, -1));
		}
	}        

	//Current master
	uint32_t masterId = GetGw();
	//Current level in Hierarchy
	uint32_t levelN = 0;
	//Number of nodes that have not assigned master
	uint32_t orphanN = cliqueS.size();


	std::vector <uint32_t> level0 (1, GetGw());
	hierarchy.push_back(level0);
	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Hierarchy<<<<<<<<<<<<<<<<<<<<<<<"); 
	NS_LOG_INFO("Level 0 : (gw)" << hierarchy.at(0).at(0));                        

	while (orphanN != 0)
	{
		levelN++;
		std::vector<uint32_t> nthLevel;
		for (uint32_t i = 0; i < hierarchy.back().size(); i++)
		{
			masterId = hierarchy.back().at(i);
			//get master cfl clique id
			std::vector<uint32_t>::iterator itMaster = std::find (conflictNodes.begin(), conflictNodes.end(), masterId);
			uint32_t idMasterClq = std::distance(conflictNodes.begin(), itMaster);


			for (std::vector<uint32_t>::iterator itNeigh = m_neighbourNodes.at(masterId).begin(); itNeigh!= m_neighbourNodes.at(masterId).end(); ++itNeigh)
			{
				//find the neighbour that is also cfl
				std::vector<uint32_t>::iterator itConflictNodes = std::find (conflictNodes.begin(), conflictNodes.end(), *itNeigh);
				uint32_t idNodeClq = std::distance(conflictNodes.begin(), itConflictNodes);

				if (itConflictNodes == conflictNodes.end())
					continue;

				//check if the cfl neighbour has been arranged
				std::map<int32_t, int32_t>::iterator it =  m_master.find(*itNeigh);

				//if not
				if (it->second == -1)
				{
					//the current master
					it->second = masterId;

					NS_LOG_INFO(" Level"<< levelN << " node "<< *itNeigh << " follows master: "<< masterId << " of clique "<< idMasterClq);        

					std::map<int32_t, int32_t>::iterator itMasterClique =  m_masterClique.find(*itNeigh);
					itMasterClique->second = idMasterClq;

					nthLevel.push_back(*itNeigh);
					//scheduling order

					m_schedulingOrder.push_back(idNodeClq);
				}
			}
		}
		//add new level
		hierarchy.push_back(nthLevel);
		//recount the number of orphaN
		orphanN = 0;
		for(std::map<int32_t, int32_t>::iterator itMaster = m_master.begin(); itMaster != m_master.end(); itMaster++) 
		{
			if(itMaster->second == -1)
				orphanN++;
		}
	}
    //Main hierarchy shall be ready upto this point
    //if simulating interfering scenario,
    //this is to configure which nodes are interfering with each other
    //and to add the interfering cliques, which shall obey to the scheduling order just configured
    //note: dimentions of clique and m_schedulingOrder will be different
    if(m_sim_interference){
        ConfigureInterferenceSets();
    }
}
    
void DmgAlmightyController::ConfigureInterferenceSets(void)
{
    m_intfStas.clear();
    for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
        for (uint32_t linkIdx = 0; linkIdx < m_linkList.size(); linkIdx++){
            uint32_t neighId;
            
            if (staIdx != m_linkList[linkIdx][0] && staIdx != m_linkList[linkIdx][1])
                continue;
            else
                neighId = (staIdx == m_linkList[linkIdx][0])?(m_linkList[linkIdx][1]):(m_linkList[linkIdx][0]);
            
            Ptr<DmgWifiMac> staMac =
            m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->
            GetMac()->GetObject<DmgWifiMac>();
            Ptr<DmgWifiMac> neiMac =
            m_meshNodes->Get(neighId)->GetDevice(0)->GetObject<WifiNetDevice>()->
            GetMac()->GetObject<DmgWifiMac>();
            
            Ptr<DmgDestinationFixedWifiManager> staManager = staMac->
            GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();
            Ptr<DmgDestinationFixedWifiManager> neiManager = neiMac->
            GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();
            
            double neiRxPower = GetIdealRxPower(m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
            double staRxPower = GetIdealRxPower(m_meshNodes->Get(neighId), m_meshNodes->Get(staIdx));
            //-----------------------measure interference for other stations
            for (uint32_t intfLinkIdx = 0; intfLinkIdx < m_linkList.size(); intfLinkIdx++){
                uint32_t aIntf = m_linkList[intfLinkIdx][0];
                uint32_t bIntf = m_linkList[intfLinkIdx][1];
                
                if (staIdx == aIntf || staIdx == bIntf || neighId == aIntf || neighId == bIntf)
                    continue;
                
                double aRxPower = GetInterferencePowerFromPair(m_meshNodes->Get(m_linkList[intfLinkIdx][0]), m_meshNodes->Get(bIntf), m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
                double bRxPower = GetInterferencePowerFromPair(m_meshNodes->Get(m_linkList[intfLinkIdx][1]), m_meshNodes->Get(aIntf), m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
                
                if (aRxPower > -1.0e6){
                    NS_LOG_UNCOND(staIdx << " to "<< neighId <<"(" << staRxPower<<")");
                    NS_LOG_UNCOND("Interference>>>>>>" << aIntf << " rx power "<< aRxPower<< " when switch to " << bIntf);
                    std::vector <uint32_t> interfSet (4);
                    interfSet[0] = staIdx;
                    interfSet[1] = neighId;
                    interfSet[2] = aIntf;
                    interfSet[3] = bIntf;
                    m_intfStas.push_back(interfSet);
                }
                if (bRxPower > -1.0e6){
                    NS_LOG_UNCOND(staIdx << " to "<< neighId <<"(" << staRxPower<<")");
                    NS_LOG_UNCOND("Interference>>>>>>" << bIntf << " rx power "<< bRxPower<< " when switch to " << aIntf);
                    std::vector <uint32_t> interfSet (4);
                    interfSet[0] = staIdx;
                    interfSet[1] = neighId;
                    interfSet[2] = bIntf;
                    interfSet[3] = aIntf;
                    m_intfStas.push_back(interfSet);
                }
                
            }
        }
    }
    //////////////////////////////////////////////////////////
    //-------------add to 'interfering cliques'
    //Mark the start of interfering clique in the array of cliques
    m_interfCliqueStart = cliqueS.size();
    for (uint32_t intf_set_id = 0; intf_set_id< m_intfStas.size(); intf_set_id ++){
        
        
        //////////////////////////////to avoid adding the same set of stations again
        bool if_recorded = false;
        if (intf_set_id > 0){
            for (uint32_t cId = m_interfCliqueStart; cId < cliqueS.size(); cId ++ ){
                uint32_t num_same = 0;
                for (uint32_t i= 0; i < m_intfStas[intf_set_id].size(); i++){
                    if (std::find(cliqueS[cId].staMem.begin(), cliqueS[cId].staMem.end(), m_intfStas[intf_set_id][i])!=  cliqueS[cId].staMem.end()){
                        num_same++;
                    }
                    if (num_same == m_intfStas[intf_set_id].size()){
                        if_recorded = true;
                    }
                }
            }
        }
        
        if(if_recorded)
            continue;
        //////////////////////////////to avoid adding the same set of stations again--end
        
        cliqueStruct c;
        c.staMem= m_intfStas[intf_set_id]; //sort the order of members so that 'conflict node' is at the end? do we need cfl node for such cliques?
        c.staMemN = m_intfStas[intf_set_id].size();
        
        cliqueS.push_back(c);
        
        uint32_t cIdx = cliqueS.size() - 1;
        NS_LOG_INFO("adding interfering clique "<< cIdx);
        for (uint32_t flowIdx=0; flowIdx < m_flowsPath.size(); flowIdx++){
                //build clique.flows and clique.flowSegs using flowPath
            for (uint32_t i = 0; i < m_flowsPath[flowIdx].size() - 1; i++){
                //add flow seg to clique if the interfering links are in the flow
                if (((cliqueS[cIdx].staMem[0] == m_flowsPath[flowIdx][i]) && (cliqueS[cIdx].staMem[1] == m_flowsPath[flowIdx][i + 1])) || ((cliqueS[cIdx].staMem[2] == m_flowsPath[flowIdx][i]) && (cliqueS[cIdx].staMem[3] == m_flowsPath[flowIdx][i + 1])) || ((cliqueS[cIdx].staMem[0] == m_flowsPath[flowIdx][i + 1]) && (cliqueS[cIdx].staMem[1] == m_flowsPath[flowIdx][i])) || ((cliqueS[cIdx].staMem[2] == m_flowsPath[flowIdx][i + 1]) && (cliqueS[cIdx].staMem[3] == m_flowsPath[flowIdx][i])) )
                {
                    NS_LOG_INFO("---- with flow "<< flowIdx);
                    cliqueS[cIdx].flows.push_back(flowIdx);
                    
                    std::vector <uint32_t> link;
                    link.push_back(m_flowsPath[flowIdx][i]);
                    link.push_back(m_flowsPath[flowIdx][i+1]);
                    
                    cliqueS[cIdx].flowSegs.push_back(link);
                    
                    for (uint32_t j = 0; j < link.size(); j++){
                        NS_LOG_INFO("between "<< link[j]);
                    }
                    cliqueS[cIdx].phyRate.push_back(0);
                    cliqueS[cIdx].timeAlloc.push_back(0);//Initiate time for each flow segment. Will be configured in progressive filling
                }
            }
        }

    }
    
/*    for ( uint32_t flowIdx=0; flowIdx < m_flowsPath.size(); flowIdx++){
        for ( uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++){
            //build clique.flows and clique.flowSegs using flowPath
            for (uint32_t i = 0; i < m_flowsPath[flowIdx].size() - 1; i++){
                //if the clique has both stations of the current link
                if((std::find(cliqueS[cIdx].staMem.begin(), cliqueS[cIdx].staMem.end(), m_flowsPath[flowIdx][i]) != cliqueS[cIdx].staMem.end()) && (std::find(cliqueS[cIdx].staMem.begin(), cliqueS[cIdx].staMem.end(), m_flowsPath[flowIdx][i+1]) != cliqueS[cIdx].staMem.end())){
                    cliqueS[cIdx].flows.push_back(flowIdx);
                    
                    std::vector <uint32_t> link;
                    link.push_back(m_flowsPath[flowIdx][i]);
                    link.push_back(m_flowsPath[flowIdx][i+1]);
                    cliqueS[cIdx].flowSegs.push_back(link);
                    
                    cliqueS[cIdx].phyRate.push_back(0);
                    cliqueS[cIdx].timeAlloc.push_back(0);//Initiate time for each flow segment.
                }
            }
        }
    }*/

    

}

	uint32_t
DmgAlmightyController::GetMasterCliqueId (uint32_t node)
{
	std::map<int32_t, int32_t>::iterator itMasterClique =  m_masterClique.find(node);
	uint32_t cliqueId = itMasterClique->second;

	return cliqueId;
}

	uint32_t
DmgAlmightyController::GetMasterNodeId (uint32_t node)
{
	std::map<int32_t, int32_t>::iterator itMaster =  m_master.find(node);
	uint32_t masterId = itMaster->second;

	return masterId;
}


//called when m_sim_interference == true
	void
DmgAlmightyController::ConfigureScheduleWithInterfAvoidance (void)
{
	NS_LOG_FUNCTION(this);
    
	uint64_t overheadDurNs = (uint64_t) ceil(m_biDuration * m_biOverhaedFraction);
	uint64_t scheduleAvailableTimeNs = (uint64_t)floor((m_biDuration - overheadDurNs)/m_numSchedulePerBi);
	uint64_t nextSpStartNs = 0;

	std::vector <uint32_t> conflictNodes;

    //Do sth with the cIdx >= m_interfCliqueStart
	for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++) {
		conflictNodes.push_back(cliqueS[cIdx].staMem.back());

		cliqueS[cIdx].bufStart.clear();
		cliqueS[cIdx].bufStart.resize(cliqueS[cIdx].flowSegs.size());
		cliqueS[cIdx].bufDurNs.clear();
		cliqueS[cIdx].bufDurNs.resize(cliqueS[cIdx].flowSegs.size());
		cliqueS[cIdx].bufStaOrder.clear(); //bufSegIndexOrder?
	}

	for (uint32_t i = 0; i < m_schedulingOrder.size(); i++) {

		uint32_t cIdx = m_schedulingOrder.at(i);

		uint32_t conflictNode = conflictNodes.at(cIdx);
		nextSpStartNs = 0;
		NS_LOG_INFO("In clique " << cIdx << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ConflictNode" << conflictNode);

		std::vector < std::vector < uint64_t > > bufCflSp;
		std::vector < std::vector < uint64_t > > timeAvailableInClique; //Time available for the clique

		uint32_t idMasterClq;//The id of master clique

		if (conflictNode != conflictNodes.front()){
			std::vector<uint32_t>::iterator it;
			//The next hop of a cfl node is the master cfl
			idMasterClq = GetMasterCliqueId(conflictNode);
			NS_LOG_INFO ( " master node "<< GetMasterNodeId(conflictNode) << " clique " <<idMasterClq );

			//Get the buffered sps from the Master Clique
			for (uint32_t segIdx = 0; segIdx < cliqueS[idMasterClq].flows.size(); segIdx++){            
				if ((conflictNode == cliqueS[idMasterClq].flowSegs[segIdx][0])||(conflictNode == cliqueS[idMasterClq].flowSegs[segIdx][1])){

                    for (uint32_t bIdx = 0; bIdx < cliqueS[idMasterClq].bufStart[segIdx].size(); bIdx++) {
						std::vector <uint64_t> spStartStop;
						spStartStop.push_back(cliqueS[idMasterClq].bufStart[segIdx][bIdx]);
						spStartStop.push_back(cliqueS[idMasterClq].bufStart[segIdx][bIdx]+cliqueS[idMasterClq].bufDurNs[segIdx][bIdx]);
						bufCflSp.push_back(spStartStop); 
						NS_LOG_INFO("CFL has buffered SP from clique"<< idMasterClq << " from "<< bufCflSp.back().at(0) <<" to "<< bufCflSp.back().at(1) << " seg "<< segIdx);
					}
				}
			}

			//Get the available time slot to aid the shuffling
			timeAvailableInClique.resize(bufCflSp.size() + 1);
			timeAvailableInClique.front().push_back(0);
			for (uint32_t iRow = 0; iRow < bufCflSp.size(); iRow++){
				timeAvailableInClique.at(iRow).push_back(bufCflSp.at(iRow).at(0));
				timeAvailableInClique.at(iRow+1).push_back(bufCflSp.at(iRow).at(1));
			}
			timeAvailableInClique.back().push_back(scheduleAvailableTimeNs);
		}
        
        for (uint32_t slot = 0; slot < timeAvailableInClique.size(); slot++)
        {
            NS_LOG_INFO("timeAvailableInClique from " << timeAvailableInClique[slot][0] << " to " << timeAvailableInClique[slot][1]);
        }

		//Slicing and buffering on STAs
		for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flowSegs.size(); segIdx++) {
			NS_LOG_INFO("Flow "<< cliqueS[cIdx].flows[segIdx] << " segment between "<<cliqueS[cIdx].flowSegs[segIdx][0] <<" and "<< cliqueS[cIdx].flowSegs[segIdx][1]);
            
            //avoiding interfering links to be active concurrently
            std::vector < std::vector < uint64_t > > timeAvailable = timeAvailableInClique;

            std::vector < std:: vector <uint64_t> > timeAvoid;
            // in interfering cliques
            for (uint32_t cItf = m_interfCliqueStart; cItf < cliqueS.size(); cItf++)
            {
                std::vector <uint32_t> segIdInClq;
                segIdInClq = GetSegIndicesInClique(cliqueS[cIdx].flowSegs[segIdx], cliqueS[cItf].flowSegs);
                if(segIdInClq.empty()){
                    continue;
                }
                //NS_LOG_INFO("interf clq"<< cItf);
                //taking care of cliqueS[cIdx].flowSegs[segIdx]
                //check if the other link in interfering clique have buffered times
                //if yes, buf this for cliqueS[cIdx].flowSegs[segIdx] to avoid later

                //check all the other segs in this interf clique
                for(uint32_t otherSeg = 0; otherSeg < cliqueS[cItf].flowSegs.size(); otherSeg++)
                {
                    if (std::find(segIdInClq.begin(),segIdInClq.end(), otherSeg) != segIdInClq.end()){
                        //skip the segs in segIdInClq
                        continue;
                    }
                    
                    std::vector <uint32_t> seg_to_avoid = cliqueS[cItf].flowSegs[otherSeg];
                    //find in classic cliques where these links belong to
                    for (uint32_t cClassic = 0; cClassic < m_interfCliqueStart; cClassic++)
                    {
                        std::vector <uint32_t> seg_avoid_in_classic;
                        
                        seg_avoid_in_classic = GetSegIndicesInClique(seg_to_avoid, cliqueS[cClassic].flowSegs);

                        if(seg_avoid_in_classic.empty()){
                            continue;
                        }
                       
                        //check if they have buffered SPs
                        for(uint32_t iter = 0; iter< seg_avoid_in_classic.size(); iter++){
                            //if buffered size is greater than 0
                            uint32_t seg = seg_avoid_in_classic[iter];
                          
                            if(cliqueS[cClassic].bufStart[seg].size()){
                                //NS_LOG_INFO("buf size"<< cliqueS[cClassic].bufStart[seg].size());
                                for(uint32_t buf_id = 0; buf_id< cliqueS[cClassic].bufStart[seg].size(); buf_id++){
                                    std::vector <uint64_t> avoidStartStop;
                                    avoidStartStop.push_back(cliqueS[cClassic].bufStart[seg][buf_id]);
                                    avoidStartStop.push_back(cliqueS[cClassic].bufStart[seg][buf_id] + cliqueS[cClassic].bufDurNs[seg][buf_id]);
                                    timeAvoid.push_back(avoidStartStop);
                                    
                                    //Print info for debugging
                                    //NS_LOG_INFO("...should avoid "<< cliqueS[cClassic].bufStart[seg][buf_id] <<" and "<< cliqueS[cClassic].bufStart[seg][buf_id] + cliqueS[cClassic].bufDurNs[seg][buf_id] << " (used by "<< seg_to_avoid[0] <<" and "<< seg_to_avoid[1] << " flow "<< cliqueS[cClassic].flows[seg] << " in clique " << cClassic <<")" );
                                }
                            }
                        }
                    }
                }
            }

            for(uint32_t i=0; i< timeAvoid.size(); i++)
            {
                for(uint32_t j=0; j<i; j++)
                {
                    if(timeAvoid[i][0] == timeAvoid[j][0] && timeAvoid[i][1] == timeAvoid[j][1])
                    {
                        timeAvoid.erase(timeAvoid.begin() + i);
                        i--;
                    }
                }
            }
            for(uint32_t i=0; i< timeAvoid.size(); i++)
                NS_LOG_INFO("To AVOID " << timeAvoid[i][0] << " "<< timeAvoid[i][1] <<" for interf");

            
            timeAvailable = RemoveIntervalFromTimeAvailable(timeAvoid, timeAvailable);
            
           
            //the actual scheduling
			uint64_t segSpDurationNs = (uint64_t) floor (scheduleAvailableTimeNs * cliqueS[cIdx].timeAlloc[segIdx]);
			bool segAllocated = 0;

			if (conflictNode == conflictNodes.front()) {

				cliqueS[cIdx].bufStart[segIdx].push_back(nextSpStartNs);
				cliqueS[cIdx].bufDurNs[segIdx].push_back(segSpDurationNs);// buffer sp on sta
				NS_LOG_INFO(" buffering time between "<< cliqueS[cIdx].bufStart[segIdx].back() <<" and "<< cliqueS[cIdx].bufStart[segIdx].back() + cliqueS[cIdx].bufDurNs[segIdx].back());
				nextSpStartNs += segSpDurationNs;
			}
			else{
				for (uint32_t segIdMaster = 0; segIdMaster < cliqueS[idMasterClq].bufStart.size(); segIdMaster++) 
				{                 
					if((cliqueS[cIdx].flowSegs[segIdx][0] == cliqueS[idMasterClq].flowSegs[segIdMaster][0]) && (cliqueS[cIdx].flowSegs[segIdx][1] == cliqueS[idMasterClq].flowSegs[segIdMaster][1]) && (cliqueS[cIdx].flows[segIdx] == cliqueS[idMasterClq].flows[segIdMaster])){

						for (uint32_t bIdx = 0; bIdx < cliqueS[idMasterClq].bufStart[segIdMaster].size(); bIdx++){
							cliqueS[cIdx].bufStart[segIdx].push_back(cliqueS[idMasterClq].bufStart[segIdMaster][bIdx]);
							cliqueS[cIdx].bufDurNs[segIdx].push_back(cliqueS[idMasterClq].bufDurNs[segIdMaster][bIdx]);

						}
						//NS_LOG_INFO("Skipping because this flow seg has been allocated SP " );
						segAllocated = 1;
					}
				}
				if(segAllocated == 1){
					continue;
				}

				uint64_t timeNeeded = segSpDurationNs;
				NS_LOG_INFO("Time needed " << timeNeeded);
				bool ifSplited = true;
				//Firstly try to fit in any time available gap without chopping
				for(uint32_t iRow = 0;iRow < timeAvailable.size() ;iRow++){
					if (segSpDurationNs <= timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0))
                    {
						cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
						cliqueS[cIdx].bufDurNs[segIdx].push_back(segSpDurationNs);// buffer sp on sta
						NS_LOG_INFO("SP fitted in between " << cliqueS[cIdx].bufStart[segIdx].back() << " and " << cliqueS[cIdx].bufDurNs[segIdx].back()+cliqueS[cIdx].bufStart[segIdx].back());
						timeAvailable.at(iRow).at(0) +=segSpDurationNs;
                        
                        //modify timeAvailableInClique
                        std::vector< std::vector< uint64_t> > timeTaken;
                        std::vector< uint64_t> fromTo;
                        fromTo.push_back(cliqueS[cIdx].bufStart[segIdx].back());//from
                        fromTo.push_back(cliqueS[cIdx].bufDurNs[segIdx].back()+cliqueS[cIdx].bufStart[segIdx].back());//to
                        timeTaken.push_back(fromTo);
                        timeAvailableInClique = RemoveIntervalFromTimeAvailable(timeTaken, timeAvailableInClique);
                        
						ifSplited = false;
						timeNeeded = 0;
						break;
					}
				}
				//If cannot fit in any gaps, fill the gaps sequentially from the 1st gap until all timeNeeded has been fitted
				if(ifSplited){
					for(uint32_t iRow = 0;iRow < timeAvailable.size() ;iRow++){
						if (timeAvailable.at(iRow).at(1) == timeAvailable.at(iRow).at(0))
							continue;

						if (timeNeeded >= timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0)){
							NS_LOG_INFO(" sp cannot fitted in between " << timeAvailable.at(iRow).at(0) << " and " << timeAvailable.at(iRow).at(1) << "(slicing)");
							cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
							cliqueS[cIdx].bufDurNs[segIdx].push_back(timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0));
							timeAvailable.at(iRow).at(0) = timeAvailable.at(iRow).at(1);
                            
                            //modify timeAvailableInClique
                            std::vector< std::vector< uint64_t> > timeTaken;
                            std::vector< uint64_t> fromTo;
                            fromTo.push_back(cliqueS[cIdx].bufStart[segIdx].back());//from
                            fromTo.push_back(cliqueS[cIdx].bufDurNs[segIdx].back() + cliqueS[cIdx].bufStart[segIdx].back());//to
                            timeTaken.push_back(fromTo);
                            timeAvailableInClique = RemoveIntervalFromTimeAvailable(timeTaken, timeAvailableInClique);
                            
							timeNeeded = timeNeeded - cliqueS[cIdx].bufDurNs[segIdx].back();
							NS_LOG_INFO(" more time needed " << timeNeeded);
						}
						else if (timeNeeded < timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0)){
							NS_LOG_INFO(" sp fitted in between " << timeAvailable.at(iRow).at(0) << " and " << timeAvailable.at(iRow).at(0) + timeNeeded << "(stop slicing)");
							cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
							cliqueS[cIdx].bufDurNs[segIdx].push_back(timeNeeded);
							timeAvailable.at(iRow).at(0) += timeNeeded;
                            
                            //modify timeAvailableInClique
                            std::vector< std::vector< uint64_t> > timeTaken;
                            std::vector< uint64_t> fromTo;
                            fromTo.push_back(cliqueS[cIdx].bufStart[segIdx].back());//from
                            fromTo.push_back(cliqueS[cIdx].bufDurNs[segIdx].back() + cliqueS[cIdx].bufStart[segIdx].back());//to
                            timeTaken.push_back(fromTo);
                            timeAvailableInClique = RemoveIntervalFromTimeAvailable(timeTaken, timeAvailableInClique);
                            
							timeNeeded = 0;
						}
					}
				}

                if (timeNeeded){
					if (timeNeeded<=10){
						timeNeeded = 0;
						//NS_LOG_INFO(" time in clique overflowing but for small amount (<=10ns)");
					}
					else{
						NS_LOG_WARN(" WARNING: time in clique" << cIdx << "overflowing");
						exit(-1);
					}
				}
			}
		}
	}
}

    //utility function to remove 'toRemove' from 'avaiTime'
std::vector < std::vector <uint64_t> >
DmgAlmightyController:: RemoveIntervalFromTimeAvailable (std::vector < std::vector <uint64_t> > toRemove, std::vector < std::vector <uint64_t> > avaiTime)
{
    for(uint32_t i=0; i< toRemove.size(); i++)
    {
        uint64_t avoidStart = toRemove[i][0];
        uint64_t avoidStop = toRemove[i][1];
        for (uint32_t slot = 0; slot < avaiTime.size(); slot++)
        {
//            NS_LOG_INFO("avaiTime from " << avaiTime[slot][0] << " to " << avaiTime[slot][1] << " avoid start "<< avoidStart << " avoid stop " << avoidStop);
            
            if(avoidStart <= avaiTime[slot][0] && avoidStop >= avaiTime[slot][0] && avoidStop <= avaiTime[slot][1])
            {
                avaiTime[slot][0] = avoidStop;
//                NS_LOG_INFO("shortening slot from front " << avaiTime[slot][0] << " to " << avaiTime[slot][1]);
            }
            else if(avoidStart >= avaiTime[slot][0] && avoidStart <= avaiTime[slot][1] && avoidStop >= avaiTime[slot][1])
            {
                avaiTime[slot][1] = avoidStart;
//                NS_LOG_INFO("shortening slot from back " << avaiTime[slot][0] << " to " << avaiTime[slot][1]);
            }
            else if(avoidStart > avaiTime[slot][0] && avoidStart < avaiTime[slot][1] && avoidStop > avaiTime[slot][0] && avoidStop < avaiTime[slot][1])
            {
//                NS_LOG_INFO("inserting new row ");
//                for(uint32_t i=0; i< avaiTime.size(); i++)
//                    NS_LOG_INFO(" before TimeAvai" << avaiTime[i][0] << " "<< avaiTime[i][1] <<" for interf");
//
                std::vector <uint64_t> newRow;
                newRow.push_back(avoidStop);
                newRow.push_back(avaiTime[slot][1]);
                
                avaiTime[slot][1] = avoidStart;
                
//                NS_LOG_INFO(" vec increasing?" << avaiTime.size());
                
                std::vector< std::vector<uint64_t > >::iterator it;
                
                it = avaiTime.begin() + slot + 1;
                it = avaiTime.insert (it , newRow); //slot++ or not?
                
//                NS_LOG_INFO(" vec increasing" << avaiTime.size());
//                for(uint32_t i=0; i< avaiTime.size(); i++)
//                        NS_LOG_INFO("TimeAvai " << avaiTime[i][0] << " "<< avaiTime[i][1] <<" for interf");
            }
            else
            {
                //NS_LOG_INFO("not changing avaiTime");
            }
        }
        
    }

    return avaiTime;
}
    
    // When m_sim_interf == false
void
DmgAlmightyController::ConfigureSchedule (void)
{
    NS_LOG_FUNCTION(this);
    
    uint64_t overheadDurNs = (uint64_t) ceil(m_biDuration * m_biOverhaedFraction);
    uint64_t scheduleAvailableTimeNs = (uint64_t)floor((m_biDuration - overheadDurNs)/m_numSchedulePerBi);
    uint64_t nextSpStartNs = 0;
    
    std::vector <uint32_t> conflictNodes;
    
    //Do sth with the cIdx >= m_interfCliqueStart
    for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++) {
        conflictNodes.push_back(cliqueS[cIdx].staMem.back());
        
        cliqueS[cIdx].bufStart.clear();
        cliqueS[cIdx].bufStart.resize(cliqueS[cIdx].flowSegs.size());
        cliqueS[cIdx].bufDurNs.clear();
        cliqueS[cIdx].bufDurNs.resize(cliqueS[cIdx].flowSegs.size());
        cliqueS[cIdx].bufStaOrder.clear(); //bufSegIndexOrder?
    }
    
    for (uint32_t i = 0; i < m_schedulingOrder.size(); i++) {
        
        uint32_t cIdx = m_schedulingOrder.at(i);
        
        uint32_t conflictNode = conflictNodes.at(cIdx);
        nextSpStartNs = 0;
        NS_LOG_INFO("In clique " << cIdx << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ConflictNode" << conflictNode);
        
        std::vector < std::vector < uint64_t > > bufCflSp;
        std::vector < std::vector < uint64_t > > timeAvailable; //Time available for the clique
        
        uint32_t idMasterClq;//The id of master clique
        
        if (conflictNode != conflictNodes.front()){
            std::vector<uint32_t>::iterator it;
            //The next hop of a cfl node is the master cfl
            idMasterClq = GetMasterCliqueId(conflictNode);
            NS_LOG_INFO ( " master node "<< GetMasterNodeId(conflictNode) << " clique " <<idMasterClq );
            
            //Get the buffered sps from the Master Clique
            for (uint32_t segIdx = 0; segIdx < cliqueS[idMasterClq].flows.size(); segIdx++){
                if ((conflictNode == cliqueS[idMasterClq].flowSegs[segIdx][0])||(conflictNode == cliqueS[idMasterClq].flowSegs[segIdx][1])){
                    for (uint32_t bIdx = 0; bIdx < cliqueS[idMasterClq].bufStart[segIdx].size(); bIdx++) {
                        std::vector <uint64_t> spStartStop;
                        spStartStop.push_back(cliqueS[idMasterClq].bufStart[segIdx][bIdx]);
                        spStartStop.push_back(cliqueS[idMasterClq].bufStart[segIdx][bIdx]+cliqueS[idMasterClq].bufDurNs[segIdx][bIdx]);
                        bufCflSp.push_back(spStartStop);
                        NS_LOG_INFO("CFL has buffered SP from clique"<< idMasterClq << " from "<< bufCflSp.back().at(0) <<" to "<< bufCflSp.back().at(1));
                    }
                }
            }
            
            //Get the available time slot to aid the shuffling
            timeAvailable.resize(bufCflSp.size() + 1);
            timeAvailable.front().push_back(0);
            
            for (uint32_t iRow = 0; iRow < bufCflSp.size(); iRow++){
                timeAvailable.at(iRow).push_back(bufCflSp.at(iRow).at(0));
                timeAvailable.at(iRow+1).push_back(bufCflSp.at(iRow).at(1));
            }
            timeAvailable.back().push_back(scheduleAvailableTimeNs);
        }
        
        //Slicing and buffering on STAs
        for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flowSegs.size(); segIdx++) {
            NS_LOG_INFO("Flow "<< cliqueS[cIdx].flows[segIdx] << " segment between "<<cliqueS[cIdx].flowSegs[segIdx][0] <<" and "<< cliqueS[cIdx].flowSegs[segIdx][1]);
            
//            for (uint32_t slot = 0; slot < timeAvailable.size(); slot++)
//            {
//                NS_LOG_INFO("timeAvailable from " << timeAvailable[slot][0] << " to " << timeAvailable[slot][1]);
//            }
            
            uint64_t segSpDurationNs = (uint64_t) floor (scheduleAvailableTimeNs * cliqueS[cIdx].timeAlloc[segIdx]);
            bool segAllocated = 0;
            
            if (conflictNode == conflictNodes.front()) {
                
                cliqueS[cIdx].bufStart[segIdx].push_back(nextSpStartNs);
                cliqueS[cIdx].bufDurNs[segIdx].push_back(segSpDurationNs);// buffer sp on sta
                NS_LOG_INFO(" buffering time between "<< cliqueS[cIdx].bufStart[segIdx].back() <<" and "<< cliqueS[cIdx].bufStart[segIdx].back() + cliqueS[cIdx].bufDurNs[segIdx].back());
                nextSpStartNs += segSpDurationNs;
            }
            else{
                for (uint32_t segIdMaster = 0; segIdMaster < cliqueS[idMasterClq].bufStart.size(); segIdMaster++)
                {
                    if((cliqueS[cIdx].flowSegs[segIdx][0] == cliqueS[idMasterClq].flowSegs[segIdMaster][0]) && (cliqueS[cIdx].flowSegs[segIdx][1] == cliqueS[idMasterClq].flowSegs[segIdMaster][1]) && (cliqueS[cIdx].flows[segIdx] == cliqueS[idMasterClq].flows[segIdMaster])){
                        
                        for (uint32_t bIdx = 0; bIdx < cliqueS[idMasterClq].bufStart[segIdMaster].size(); bIdx++){
                            cliqueS[cIdx].bufStart[segIdx].push_back(cliqueS[idMasterClq].bufStart[segIdMaster][bIdx]);
                            cliqueS[cIdx].bufDurNs[segIdx].push_back(cliqueS[idMasterClq].bufDurNs[segIdMaster][bIdx]);
                            
                        }
                        //NS_LOG_INFO("Skipping because this flow seg has been allocated SP " );
                        segAllocated = 1;
                    }
                }
                if(segAllocated == 1){
                    continue;
                }
                
                uint64_t timeNeeded = segSpDurationNs;
                NS_LOG_INFO("Time needed "<< timeNeeded );
                bool ifSplited = true;
                //Firstly try to fit in any time available gap without chopping
                for(uint32_t iRow = 0;iRow < timeAvailable.size() ;iRow++){
                    if (segSpDurationNs <= timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0)){
                        cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
                        cliqueS[cIdx].bufDurNs[segIdx].push_back(segSpDurationNs);// buffer sp on sta
                        NS_LOG_INFO("SP fitted in between " << cliqueS[cIdx].bufStart[segIdx].back() << " and " << cliqueS[cIdx].bufDurNs[segIdx].back()+cliqueS[cIdx].bufStart[segIdx].back());
                        timeAvailable.at(iRow).at(0) +=segSpDurationNs;
                        ifSplited = false;
                        timeNeeded = 0;
                        break;
                    }
                }
                //If cannot fit in any gaps, fill the gaps sequentially from the 1st gap until all timeNeeded has been fitted
                if(ifSplited){
                    for(uint32_t iRow = 0;iRow < timeAvailable.size() ;iRow++){
                        if (timeAvailable.at(iRow).at(1) == timeAvailable.at(iRow).at(0))
                            continue;
                        
                        if (timeNeeded >= timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0)){
                            NS_LOG_INFO(" sp cannot fitted in between " << timeAvailable.at(iRow).at(0) << " and " << timeAvailable.at(iRow).at(1) << "(slicing)");
                            cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
                            cliqueS[cIdx].bufDurNs[segIdx].push_back(timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0));
                            timeAvailable.at(iRow).at(0) = timeAvailable.at(iRow).at(1);
                            timeNeeded = timeNeeded - cliqueS[cIdx].bufDurNs[segIdx].back();
                            NS_LOG_INFO(" more time needed " << timeNeeded);
                        }
                        else if (timeNeeded < timeAvailable.at(iRow).at(1) - timeAvailable.at(iRow).at(0)){
                            NS_LOG_INFO(" sp fitted in between " << timeAvailable.at(iRow).at(0) << " and " << timeAvailable.at(iRow).at(0) + timeNeeded << "(stop slicing)");
                            cliqueS[cIdx].bufStart[segIdx].push_back(timeAvailable.at(iRow).at(0));
                            cliqueS[cIdx].bufDurNs[segIdx].push_back(timeNeeded);
                            timeAvailable.at(iRow).at(0) += timeNeeded;
                            timeNeeded = 0;
                        }
                    }
                }
                if (timeNeeded){
                    if (timeNeeded<=10){
                        timeNeeded = 0;
                        //NS_LOG_INFO(" time in clique overflowing but for small amount (<=10ns)");
                    }
                    else{
                        NS_LOG_WARN(" WARNING: time in clique" << cIdx << "overflowing");
                        exit(-1);
                    }
                }
            }
        }
    }
}

	void
DmgAlmightyController:: ConfigureBeaconIntervals (void)
{
    //exit(-1);
	NS_LOG_FUNCTION(this);

	// Set beacon interval duration on all nodes
	for (uint32_t i = 0; i < m_meshNodes->GetN(); i++) {
		m_meshNodes->Get(i)->GetDevice(0)->GetObject<WifiNetDevice>()->
			GetMac()->GetObject<DmgWifiMac>()->
			GetDmgBeaconInterval()->SetBiDuration(NanoSeconds(m_biDuration));
	}
	// Prepare the conflictNodes vector
	std::vector <uint32_t> conflictNodes;
	for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++) {
		conflictNodes.push_back(cliqueS[cIdx].staMem.back());
	}
	//Erase Previous SPs on all nodes
	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
		Ptr<DmgBeaconInterval> dmgBiSta= m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval();
		dmgBiSta->EraseSp();
	}

	uint64_t overheadDurNs = (uint64_t) ceil(m_biDuration * m_biOverhaedFraction);
	uint64_t scheduleStartNs = overheadDurNs;
	uint64_t scheduleDurNs = (uint64_t) floor((m_biDuration - overheadDurNs)/m_numSchedulePerBi);

	//Install the schedule multiple times
	for (uint32_t scheIdx=0; scheIdx < m_numSchedulePerBi; scheIdx++){
                if(scheIdx ==0)
		NS_LOG_INFO("<<<<<<<<<< Example schedule >>> Starts at "<< scheduleStartNs <<" Ends at "<< scheduleStartNs + scheduleDurNs<<" >>>>>>>>>>>>>>>>>>>>>>");
                else
		NS_LOG_INFO("<<<<<<<<<< schedule >>> Starts at "<< scheduleStartNs <<" Ends at "<< scheduleStartNs + scheduleDurNs<<" >>>>>>>>>>>>>>>>>>>>>>");

		for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++){
			uint32_t conflictNode = conflictNodes.at(cIdx);
                        if(scheIdx ==0)
		        NS_LOG_INFO(" Start installing in clique "<< cIdx);

			for (uint32_t cliqMember = 0; cliqMember < cliqueS[cIdx].staMem.size(); cliqMember++) {

				uint32_t staIdx = cliqueS[cIdx].staMem[cliqMember];
				if(staIdx == conflictNode){
					//For conflict node, sort and install
                                        if(scheIdx ==0)
					NS_LOG_INFO("STA " << staIdx << " is the Cfl of current clique");
					std::vector <uint32_t> segIdOrder;
					std::vector <uint32_t> bufIdOrder;
					std::vector <uint64_t> refStartTime;
					for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
						for (uint32_t bIdx = 0; bIdx < cliqueS[cIdx].bufStart[segIdx].size(); bIdx++){
							segIdOrder.push_back(segIdx);
							bufIdOrder.push_back(bIdx);
							refStartTime.push_back(cliqueS[cIdx].bufStart[segIdx][bIdx]);
						}
					}

                                        if (refStartTime.size() > 1){
					        for (uint32_t segIdx = 0; segIdx < refStartTime.size() - 1; segIdx++){
						        for (uint32_t sortingIdx = segIdx + 1; sortingIdx < refStartTime.size(); sortingIdx++) {
							        if (refStartTime[segIdx] > refStartTime[sortingIdx]) {
								        std::swap(segIdOrder.at(sortingIdx), segIdOrder.at(segIdx));
								        std::swap(bufIdOrder.at(sortingIdx), bufIdOrder.at(segIdx));
								        std::swap(refStartTime.at(sortingIdx), refStartTime.at(segIdx));
							        }
						        }
					        }
                                        }
					for (uint32_t i = 0; i < refStartTime.size(); i++){
						uint32_t segId = segIdOrder[i];
						uint32_t bufId = bufIdOrder[i];
						uint32_t staId = (conflictNode == cliqueS[cIdx].flowSegs[segId][0])?(cliqueS[cIdx].flowSegs[segId][1]):(cliqueS[cIdx].flowSegs[segId][0]);

						Ptr<DmgBeaconInterval> dmgBiCfl = m_meshNodes->Get(conflictNode)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval();

						Ptr<MobilityModel> mobSta = m_meshNodes->Get(staId)->
							GetObject<MobilityModel> ();

						Mac48Address macSta = m_meshNodes->Get(staId)->GetDevice(0)->
							GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->
							GetAddress();

						uint32_t flowSink = m_flowsPath.at(cliqueS[cIdx].flows[segId]).back();
						uint32_t flowSrc = m_flowsPath.at(cliqueS[cIdx].flows[segId]).front();
						Ipv4Address ipSink = m_meshNodes->Get(flowSink)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
						Ipv4Address ipSrc = m_meshNodes->Get(flowSrc)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();

						Time spStart = NanoSeconds(cliqueS[cIdx].bufStart[segId][bufId] + scheduleStartNs);
						Time spStop = NanoSeconds(cliqueS[cIdx].bufStart[segId][bufId] + cliqueS[cIdx].bufDurNs[segId][bufId] + scheduleStartNs);
						Time subSpEnd = spStart + NanoSeconds((uint64_t) floor (cliqueS[cIdx].bufDurNs[segId][bufId]* (1 - m_ackTimeFrac)));

						bool cflIfTx = (conflictNode == cliqueS[cIdx].flowSegs[segId][0])?(true):(false);
						if (m_ackTimeFrac != 0){
							dmgBiCfl->AddSp(spStart, subSpEnd, macSta, ipSrc, ipSink, mobSta, cflIfTx);
							dmgBiCfl->AddSp(subSpEnd, spStop, macSta, ipSink, ipSrc, mobSta, 1 - cflIfTx);//ACK packets are from flow sink to flow src
						}
						else
							dmgBiCfl->AddSp(spStart, spStop, macSta, ipSrc, ipSink, mobSta, cflIfTx);
                                                if(scheIdx ==0)
						NS_LOG_INFO(" Add SP on Cfl "<< conflictNode <<" for STA" << staId << " from " << spStart << " to "<< spStop << ". Is the cfl Tx? " << cflIfTx <<" sink "<< ipSink <<" (" << flowSink <<")");
					}
				}
				else if (std::find(conflictNodes.begin(), conflictNodes.end(), staIdx) != conflictNodes.end()){
                                        if(scheIdx ==0)
					NS_LOG_INFO("STA" << staIdx << " is Cfl of other clique (skipping)");
					continue;
				}
				else{
                    //if(scheIdx ==0)
					//the sta is not cfl
					NS_LOG_INFO("STA " << staIdx << " isn't a Cfl");

					std::vector <uint32_t> segIdOrder;
					std::vector <uint32_t> bufIdOrder;
					std::vector <uint64_t> refStartTime;
					for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flowSegs.size(); segIdx++){
						if ((cliqueS[cIdx].flowSegs[segIdx][0] == staIdx)||(cliqueS[cIdx].flowSegs[segIdx][1] == staIdx)){
							NS_LOG_INFO("STA " << staIdx << "is in seg "<< segIdx << " buffer size" <<cliqueS[cIdx].bufStart[segIdx].size());
							for (uint32_t bIdx = 0; bIdx < cliqueS[cIdx].bufStart[segIdx].size(); bIdx++){
								NS_LOG_INFO("   the " << bIdx << " buffer is noted." );
								segIdOrder.push_back(segIdx);
								bufIdOrder.push_back(bIdx);
								refStartTime.push_back(cliqueS[cIdx].bufStart[segIdx][bIdx]);
							}
						}
					}
                    //NS_LOG_INFO(" "<< cliqueS[cIdx].bufStart[1][bIdx]);
                    NS_LOG_INFO(" refStartTime.size() "<< refStartTime.size());
                    //exit(-1);
                                        if (refStartTime.size() > 1){
					        for (uint32_t segIdx = 0; segIdx < refStartTime.size() - 1; segIdx++){
						        NS_LOG_INFO(" sorting in seg "<< segIdx);
						        for (uint32_t sortingIdx = segIdx + 1; sortingIdx < refStartTime.size(); sortingIdx++) {
							        if (refStartTime[segIdx] > refStartTime[sortingIdx]) {
								        std::swap(segIdOrder.at(sortingIdx), segIdOrder.at(segIdx));
								        std::swap(bufIdOrder.at(sortingIdx), bufIdOrder.at(segIdx));
								        std::swap(refStartTime.at(sortingIdx), refStartTime.at(segIdx));
							        }
						        }
					        }
                                        }
					for (uint32_t i = 0; i < refStartTime.size(); i++){
						uint32_t segId = segIdOrder[i];
						uint32_t bufId = bufIdOrder[i];

						Ptr<DmgBeaconInterval> dmgBiSta = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval();

						Ptr<MobilityModel> mobCfl = m_meshNodes->Get(conflictNode)->
							GetObject<MobilityModel> ();

						Mac48Address macCfl = m_meshNodes->Get(conflictNode)->GetDevice(0)->
							GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->
							GetAddress();

						uint32_t flowSink = m_flowsPath.at(cliqueS[cIdx].flows[segId]).back();
						uint32_t flowSrc = m_flowsPath.at(cliqueS[cIdx].flows[segId]).front();
						Ipv4Address ipSink = m_meshNodes->Get(flowSink)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
						Ipv4Address ipSrc = m_meshNodes->Get(flowSrc)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();

						Time spStart = NanoSeconds(cliqueS[cIdx].bufStart[segId][bufId] + scheduleStartNs);
						Time spStop = NanoSeconds(cliqueS[cIdx].bufStart[segId][bufId] + cliqueS[cIdx].bufDurNs[segId][bufId] + scheduleStartNs);
						Time subSpEnd = spStart + NanoSeconds((uint64_t) floor (cliqueS[cIdx].bufDurNs[segId][bufId]* (1 - m_ackTimeFrac)));

						bool staIfTx = (staIdx == cliqueS[cIdx].flowSegs[segId][0])?(true):(false);
						if (m_ackTimeFrac != 0){
							dmgBiSta->AddSp(spStart, subSpEnd, macCfl, ipSrc, ipSink, mobCfl, staIfTx);
							dmgBiSta->AddSp(subSpEnd, spStop, macCfl, ipSink, ipSrc, mobCfl, 1 - staIfTx);
						}
						else
							dmgBiSta->AddSp(spStart, spStop, macCfl, ipSrc, ipSink, mobCfl, staIfTx);

                                                if(scheIdx ==0)
						NS_LOG_INFO(" Add SP on STA "<< staIdx <<" from "<< spStart<<" to "<< spStop << " Tx? " << staIfTx  <<" sink "<< ipSink <<" (" << flowSink <<")");
					}//end for()
				}//end else()
			}//end clique Mem
		}//end clique        
		scheduleStartNs += scheduleDurNs;
	}

	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) 
	{
		Ptr<DmgBeaconInterval> dmgBiSta = m_meshNodes->Get(staIdx)->GetDevice(0)->
			GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval();

		m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->SetDmgBeaconInterval(dmgBiSta);
		dmgBiSta->SetBeamSwitchOverhead(NanoSeconds(m_beamSwitchOverheadNs));
		dmgBiSta->ScheduleNextAntennaAlignment();

		if (m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval()-> GetSps().size() != 0) 
		{
			m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>()->StartDmgSpTracking();
		}
	}
}

/* Utility function used internally to this module to configure the
 * WifiRemoteStationManager
 */
	WifiMode
DmgAlmightyController::GetWifiMode (double rxPowerDbi, bool ofdm)
{
	std::string ret = "VHTMCS0";

	if (ofdm) {
		if (rxPowerDbi >= -47)
			ret = "VHTMCS24a";
		else if (rxPowerDbi >= -49)
			ret = "VHTMCS23a";
		else if (rxPowerDbi >= -51)
			ret = "VHTMCS22a";
		else if (rxPowerDbi >= -53)
			ret = "VHTMCS21a";
		else if (rxPowerDbi >= -54)
			ret = "VHTMCS20a";
		else if (rxPowerDbi >= -56)
			ret = "VHTMCS19a";
		else if (rxPowerDbi >= -58)
			ret = "VHTMCS18a";
		else if (rxPowerDbi >= -60)
			ret = "VHTMCS17a";
		else if (rxPowerDbi >= -62)
			ret = "VHTMCS16a";
		else if (rxPowerDbi >= -63)
			ret = "VHTMCS15a";
		else if (rxPowerDbi >= -64)
			ret = "VHTMCS14a";
		else if (rxPowerDbi >= -66)
			ret = "VHTMCS13a";

	} else {
		if (rxPowerDbi >= -53)
			ret = "VHTMCS12";
		else if (rxPowerDbi >= -54)
			ret = "VHTMCS11";
		else if (rxPowerDbi >= -55)
			ret = "VHTMCS10";
		else if (rxPowerDbi >= -59)
			ret = "VHTMCS9";
		else if (rxPowerDbi >= -61)
			ret = "VHTMCS8";
		else if (rxPowerDbi >= -62)
			ret = "VHTMCS7";
		//ret = "VHTMCS5";
		else if (rxPowerDbi >= -63)
			ret = "VHTMCS6";
		else if (rxPowerDbi >= -64)
			ret = "VHTMCS4";
		else if (rxPowerDbi >= -65)
			ret = "VHTMCS3";
		else if (rxPowerDbi >= -66)
			ret = "VHTMCS2";
		else if (rxPowerDbi >= -68)
			ret = "VHTMCS1";
	}

	return ret;
}

/* Configure, on each node, the ideal Tx MCS for each possible destination
   Basically to call AddDestinationWifiMode */
	void
DmgAlmightyController::ConfigureWifiManager (void)
{
	ConfigureAntennaAlignment();
	//to be go through the link list
	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
		for (uint32_t linkIdx = 0; linkIdx < m_linkList.size(); linkIdx++){

			uint32_t neighId;

			if (staIdx != m_linkList[linkIdx][0] && staIdx != m_linkList[linkIdx][1])
				continue;
			else 
				neighId = (staIdx == m_linkList[linkIdx][0])?(m_linkList[linkIdx][1]):(m_linkList[linkIdx][0]);

			Ptr<DmgWifiMac> staMac =
				m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();
			Ptr<DmgWifiMac> neiMac =
				m_meshNodes->Get(neighId)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();

			Ptr<DmgDestinationFixedWifiManager> staManager = staMac->
				GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();
			Ptr<DmgDestinationFixedWifiManager> neiManager = neiMac->
				GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();

			double neiRxPower = GetIdealRxPower(m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
			double staRxPower = GetIdealRxPower(m_meshNodes->Get(neighId), m_meshNodes->Get(staIdx));

			//NS_LOG_UNCOND("sta rx power" << staRxPower);



			bool dmgOfdm = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetPhy()->GetObject<YansWifiPhy> () ->GetDmgOfdm();

			WifiMode neiWifiMode = GetWifiMode(staRxPower, dmgOfdm);
			WifiMode staWifiMode = GetWifiMode(neiRxPower, dmgOfdm);

			Mac48Address neiMacAddr = neiMac->GetAddress();
			Mac48Address staMacAddr = staMac->GetAddress();

			staManager->AddDestinationWifiMode(neiMacAddr, staWifiMode);
			neiManager->AddDestinationWifiMode(staMacAddr, neiWifiMode);

			if(staWifiMode.GetUniqueName() == "VHTMCS0"){
				NS_LOG_UNCOND( "ERROR: Mesh node "<< staIdx << " is in mode MCS 0 with node" << neighId);
				if (neiRxPower <= m_energyDetectionThreshold)
				{
					NS_LOG_INFO( "Node "<< neighId << "is out of node "<< staIdx << "'s range." );
				}
				exit(-1);
			}
		}
	}
}
    


double DmgAlmightyController::GetIdealRxPower(Ptr<Node> from, Ptr<Node> to)
{
	//from node: sta. to node: nei
	Ptr<YansWifiPhy> staPhy =
		from->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();
	Ptr<YansWifiPhy> neiPhy =
		to->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();

	Ptr<DmgAntennaController> staAntennaController = from->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetMac()->GetObject<DmgWifiMac>() ->GetDmgAntennaController();
	Ptr<DmgAntennaController> neiAntennaController = to->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetMac()->GetObject<DmgWifiMac>()->GetDmgAntennaController();

	Ptr<MobilityModel> staMob = from->GetObject<MobilityModel> ();
	Ptr<MobilityModel> neiMob = to->GetObject<MobilityModel> ();

	Ptr<PropagationLossModel> loss = staPhy->GetChannel()->
		GetObject<YansWifiChannel>()->GetPropagationLossModel();

	double azimuthStaAndNei = CalculateAzimuthAngle(staMob->GetPosition(),
			neiMob->GetPosition());
	double elevationStaAndNei = CalculateElevationAngle(staMob->GetPosition(),
			neiMob->GetPosition());

	//Force antenna point
	staAntennaController->PointAntenna(neiPhy);
	neiAntennaController->PointAntenna(staPhy);

	//neigh rx power from sta
	double rxPower = loss->CalcRxPower(staPhy->GetTxPowerStart() +
			staPhy->GetTxGain(), staMob, neiMob) +
		staAntennaController->GetTxGainDbi(azimuthStaAndNei, elevationStaAndNei) +
		neiAntennaController->GetRxGainDbi(azimuthStaAndNei + M_PI, -elevationStaAndNei) +
		neiPhy->GetRxGain();

	return rxPower;
}


double DmgAlmightyController::GetInterferencePowerFromPair (Ptr<Node> victim, Ptr<Node> partner, Ptr<Node> node1, Ptr<Node> node2)
{
	//interfered nodes pointing antennas to each other
	Ptr<DmgAntennaController> aAntennaController = victim->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgAntennaController();
	Ptr<DmgAntennaController> bAntennaController = partner->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgAntennaController();

	Ptr<YansWifiPhy> aPhy = victim->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();
	Ptr<YansWifiPhy> bPhy = partner->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();

	aAntennaController->PointAntenna(bPhy);
	bAntennaController->PointAntenna(aPhy);

	//interfering nodes pointing antennas to each other
	Ptr<YansWifiPhy> staPhy = node1->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();
	Ptr<YansWifiPhy> neiPhy = node2->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> ();

	Ptr<DmgAntennaController> staAntennaController = node1->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgAntennaController();
	Ptr<DmgAntennaController> neiAntennaController = node2->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgAntennaController();

	staAntennaController->PointAntenna(neiPhy);
	neiAntennaController->PointAntenna(staPhy);

	//receive power of interferencing signal at a, from the communicating pair, when sta's transmitting
	Ptr<MobilityModel> aMob = victim->GetObject<MobilityModel> ();
	Ptr<MobilityModel> staMob = node1->GetObject<MobilityModel> ();

	double azimuthSta2a = CalculateAzimuthAngle(staMob->GetPosition(),
			aMob->GetPosition());
	double elevationSta2a = CalculateElevationAngle(staMob->GetPosition(),
			aMob->GetPosition());

	Ptr<PropagationLossModel> loss = staPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel();
	double victimRxPower = loss->CalcRxPower(staPhy->GetTxPowerStart() +
			staPhy->GetTxGain(), staMob, aMob) +
		staAntennaController->GetTxGainDbi(azimuthSta2a, elevationSta2a) +
		aAntennaController->GetRxGainDbi(azimuthSta2a + M_PI, -elevationSta2a) +
		aPhy->GetRxGain();

	return victimRxPower;
}

void DmgAlmightyController::EnforceAdditionalSignalLossBetween(Ptr<Node> node1, Ptr<Node> node2, double addLoss)
{
	double node1RxPower = GetIdealRxPower(node2, node1);
	double node2RxPower = GetIdealRxPower(node1, node2);

	bool dmgOfdm1 = node1->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> () ->GetDmgOfdm();
	bool dmgOfdm2 = node2->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetPhy()->GetObject<YansWifiPhy> () ->GetDmgOfdm();

	NS_LOG_INFO("Node "<< node1->GetId() << " and Node "<< node2->GetId() <<" rx power "<<node1RxPower <<" and "<< node2RxPower);

	WifiMode n1WifiMode = GetWifiMode(node2RxPower - addLoss, dmgOfdm1);
	WifiMode n2WifiMode = GetWifiMode(node1RxPower - addLoss, dmgOfdm2);

	if(node1RxPower - addLoss <= -66)
		NS_LOG_WARN("node1RxPower - addLoss <= -66");

	NS_LOG_INFO("  .reduced to " << node1RxPower - addLoss << " and "<< node2RxPower - addLoss);
	NS_LOG_INFO("  ..wifi mode " << n1WifiMode.GetUniqueName() << " and "<< n2WifiMode.GetUniqueName());

	Ptr<DmgWifiMac> n1Mac = node1->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetMac()->GetObject<DmgWifiMac>();
	Ptr<DmgWifiMac> n2Mac = node2->GetDevice(0)->GetObject<WifiNetDevice>()->
		GetMac()->GetObject<DmgWifiMac>();

	Ptr<DmgDestinationFixedWifiManager> n1Manager = n1Mac->
		GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();
	Ptr<DmgDestinationFixedWifiManager> n2Manager = n2Mac->
		GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>();

	n1Manager->AddDestinationWifiMode(n2Mac->GetAddress(), n1WifiMode);
	n2Manager->AddDestinationWifiMode(n1Mac->GetAddress(), n2WifiMode);
}


double DmgAlmightyController::GetActualTxDurationNs(WifiMode mode, uint32_t nMpdus)
{
	double ret = 0.0;
	//if (ofdm) {// OFDM ONLY FOR NOW
	if (mode == std::string("VHTMCS24a"))
        {
                if (nMpdus == 30)
		        ret = 65291;
                else if (nMpdus == 10)
		        ret = 26107;
                else if (nMpdus == 5)
		        ret = 16880;
                else if (nMpdus == 1)
		        ret = 9624;
        }
	else if (mode == std::string("VHTMCS23a"))
        {
                if (nMpdus == 30)
		        ret = 69883;
                else if (nMpdus == 10)
                        ret = 27659;
                else if (nMpdus == 5)
		        ret = 17683;
                else if (nMpdus == 1)
		        ret = 9942;
        }	
        else if (mode == std::string("VHTMCS22a"))
        {
                if (nMpdus == 30)
		ret = 81779;
                else if (nMpdus == 10)
                        ret = 31582;
                else if (nMpdus == 5)
		        ret = 19728;
                else if (nMpdus == 1)
		        ret = 10282;
        }	
        else if (mode == std::string("VHTMCS21a"))
        {
                if (nMpdus == 30)
		        ret = 92700;
                else if (nMpdus == 10)
                        ret = 35332;
                else if (nMpdus == 5)
		        ret = 21745;
                else if (nMpdus == 1)
		        ret = 10836;
        }
	else if (mode == std::string("VHTMCS20a"))
        {
                if (nMpdus == 30)
		        ret = 99481;
                else if (nMpdus == 10)
                        ret = 37767;
                else if (nMpdus == 5)
		        ret = 22999;
                else if (nMpdus == 1)
		        ret = 11136;
        }
	else if (mode == std::string("VHTMCS19a"))
        {
                if (nMpdus == 30)
		        ret = 117180;
                else if (nMpdus == 10)
                        ret = 43713;
                else if (nMpdus == 5)
                        ret = 26071;
                else if (nMpdus == 1)
		        ret = 11698;
        }	
        else if (mode == std::string("VHTMCS18a"))
        {
                if (nMpdus == 30)
		        ret = 143830;
                else if (nMpdus == 10)
                        ret = 52724;
                else if (nMpdus == 5)
                        ret = 30454;
                else if (nMpdus == 1)
		        ret = 12782;
        }
	/*else if (mode == std::string("VHTMCS17a"))
        {
                if (nMpdus == 30)
		ret = 188435;
                else if (nMpdus == 1)
		        ret = 9624;
	}
        else if (mode == std::string("VHTMCS16a"))
        {
                if (nMpdus == 30)
		ret = 223878;
                else if (nMpdus == 1)
		        ret = 9624;
	}
        else if (mode == std::string("VHTMCS15a"))
        {
                if (nMpdus == 30)
		ret = 277194;
                else if (nMpdus == 1)
		        ret = 9624;
	}
        else if (mode == std::string("VHTMCS14a"))
        {
                if (nMpdus == 30)
		ret = 437174;
                else if (nMpdus == 1)
		        ret = 9624;
        }
	else if (mode == std::string("VHTMCS13a"))
        {
                if (nMpdus == 30)
		ret = 543726;
                else if (nMpdus == 1)
		        ret = 9624;
        }*/
	return ret;
}

	void
DmgAlmightyController::ConfigurePhyRate(uint32_t appPayloadBytes, double biOverheadFraction, uint32_t nMpdus)
{
	NS_LOG_FUNCTION(this);
	for (uint32_t cIdx = 0; cIdx< cliqueS.size(); cIdx++){

        if (m_sim_interference && (cIdx >= m_interfCliqueStart)){
            NS_LOG_INFO("In interfeing clique "<< cIdx);
        }
		for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
			uint32_t neiIdx = cliqueS[cIdx].flowSegs[segIdx][1];
			uint32_t staIdx = cliqueS[cIdx].flowSegs[segIdx][0];
			Mac48Address neiMacAddr = m_meshNodes->Get(neiIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetAddress();
			WifiMode sta2NeiWifiMode = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()-> GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>()->GetDestinationWifiMode(neiMacAddr);

			//num of MPDUs = 30; hard coded tx duration in GetActualTxDurationNs();
			//equivalent phy rate as seen from mac
			//cliqueS[cIdx].phyRate[segIdx] = sta2NeiWifiMode.GetDataRate()/1e6;
                        Time duration = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetNMpduReturnDuration(neiMacAddr);

/*                        if(duration.GetNanoSeconds())
                        {
                                cliqueS[cIdx].phyRate[segIdx] = double(nMpdus * (appPayloadBytes + 36) * 8 )/ duration.GetNanoSeconds() *1e3 *(1.0 - biOverheadFraction);
                                NS_LOG_INFO("Using measured phy rate");
                        }
                        else*/
                        {
                                NS_LOG_INFO("Using hard coded phy rate");
                                cliqueS[cIdx].phyRate[segIdx] = double(nMpdus * (appPayloadBytes + 36) * 8 )/GetActualTxDurationNs(sta2NeiWifiMode, nMpdus)*1e3 *(1.0 - biOverheadFraction);//sta2NeiWifiMode.GetDataRate()/1e6;
                        }

			NS_LOG_INFO("Equiv phy rate between "<< staIdx << " and " << neiIdx << " is " << cliqueS[cIdx].phyRate[segIdx] << "Mb/s. wifi mode" << sta2NeiWifiMode.GetUniqueName());
		}
	}
}

	std::vector <double>
DmgAlmightyController::FlowRateProgressiveFilling(std::vector <double> flowsDmd, double fillingSteplength, uint32_t appPayloadBytes, double biOverheadFraction, uint32_t nMpdus)
{
	NS_LOG_FUNCTION(this);

	ConfigurePhyRate(appPayloadBytes, biOverheadFraction, nMpdus);

	std::vector <bool> ifActive (flowsDmd.size(), true);
	std::vector <double> flowsRate (flowsDmd.size(), 0.0);
	//Initiate
	uint32_t nActive = flowsDmd.size();

	//Fill
	while (nActive!=0){
		//for each active flow, increase the rate and check if demand is met
		for (uint32_t fIdx = 0; fIdx < flowsDmd.size(); fIdx++){
			if (ifActive[fIdx]){
				// increase the rate
				flowsRate[fIdx] = flowsRate[fIdx]+fillingSteplength;
				// if demand is met, turn status to inactive
				if(flowsRate[fIdx] >= flowsDmd[fIdx]){
					ifActive[fIdx] = false;
					flowsRate[fIdx] = flowsDmd[fIdx];
					NS_LOG_INFO("Flow "<< fIdx <<" is satisfied");
				}
			}
		}

		//for each clique
		for (uint32_t cIdx = 0; cIdx< cliqueS.size(); cIdx++){
			double timeSum = 0.0;
			//calculate time fraction on each link segment: time fraction = flowrate / capacity
			for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
				cliqueS[cIdx].timeAlloc[segIdx] = flowsRate[cliqueS[cIdx].flows[segIdx]]/cliqueS[cIdx].phyRate[segIdx];
				timeSum += cliqueS[cIdx].timeAlloc[segIdx];
			}

			//check if time is used up
			if (timeSum >= 1.0){
				//NS_LOG_INFO("Time in clique "<< cIdx <<" is fully used");
				//rate and time of activeflows needed to be reallocate,
				double timeLeft = 1.0;
				double sum = 0.0;// sum = sum (1/capacities)
				//Calculated rate to be allocated
				//1. time left by inactive flows
				for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
					if (ifActive[cliqueS[cIdx].flows[segIdx]] == false) {
						timeLeft = timeLeft - cliqueS[cIdx].timeAlloc[segIdx];
						//flowsRate[cliqueS[cIdx].flows[segIdx]]/cliqueS[cIdx].phyRate[segIdx];//
					}
					else sum = sum + 1.0/(cliqueS[cIdx].phyRate[segIdx]);  //sum of (1/capacity)
				}
				//2. new rate = timeleft/sum(1/capacities)
				//NS_LOG_INFO("newRate "<< timeLeft/sum);
				double newRate = std::floor(timeLeft/sum * 10000.0)/10000.0;
				//NS_LOG_INFO("newRate "<< newRate);
				//Reallocate for active (soon to be inactive) flows same rate
				for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
					if (ifActive[cliqueS[cIdx].flows[segIdx]] == true) {
						flowsRate[cliqueS[cIdx].flows[segIdx]] = newRate;
						cliqueS[cIdx].timeAlloc[segIdx] = newRate / (cliqueS[cIdx].phyRate[segIdx]);
						ifActive[cliqueS[cIdx].flows[segIdx]] = false;
					}
				}
			}
		}
		//Re-compute the number of active flows
		nActive = 0;
		for (uint32_t fIdx = 0; fIdx< flowsDmd.size(); fIdx++){
			if(ifActive[fIdx] == true)
				nActive++;
		}
	}//end while;

	//double check time
	for (uint32_t cIdx = 0; cIdx< cliqueS.size(); cIdx++){
		double timeSum = 0.0;
		for (uint32_t segIdx = 0; segIdx < cliqueS[cIdx].flows.size(); segIdx++){
			cliqueS[cIdx].timeAlloc[segIdx] = flowsRate[cliqueS[cIdx].flows[segIdx]]/cliqueS[cIdx].phyRate[segIdx];
			timeSum += cliqueS[cIdx].timeAlloc[segIdx];
			NS_LOG_INFO("Flow "<< cliqueS[cIdx].flows[segIdx]<< " between " << cliqueS[cIdx].flowSegs[segIdx][0] <<" and " << cliqueS[cIdx].flowSegs[segIdx][1] <<" is allocated time "<< cliqueS[cIdx].timeAlloc[segIdx]);
		}
		NS_LOG_INFO("Time used in clique " << cIdx << ": " << timeSum);
        if (m_sim_interference && (cIdx >= m_interfCliqueStart)){
            NS_LOG_INFO("(Interfeing clique "<< cIdx<< ")");
        }

	}

	for (uint32_t fIdx = 0; fIdx< flowsDmd.size(); fIdx++){
		NS_LOG_INFO("Flow "<< fIdx << "'s rate: "<< flowsRate[fIdx]);
	}
	return flowsRate;
}

	void
DmgAlmightyController::AssignEqualAirTime(void)
{
	std::vector < std::vector <double> > spAlloc (m_meshNodes->GetN(), std::vector<double>(m_meshNodes->GetN()));
	// All mesh stations within a clique will be assigned the same amount of air-time
	for (uint32_t cIdx = 0; cIdx < cliqueS.size(); cIdx++) {
		uint32_t conflictNode = cliqueS[cIdx].staMem.back();// The last member of each clique is the conflict node
		double spForThisClique = 1.0/cliqueS.size()/(cliqueS[cIdx].staMem.size()-1);
		NS_LOG_INFO("Time fraction allocated for each node of clique "<< cIdx << " is " << spForThisClique );
		for (std::vector<uint32_t>::iterator it = cliqueS[cIdx].staMem.begin() ; it != cliqueS[cIdx].staMem.end()-1; it++){
			spAlloc[conflictNode][*it] = spForThisClique;
			spAlloc[*it][conflictNode] = spForThisClique;
		}
	}
	m_spAlloc = spAlloc;
}


	std::vector <double>
DmgAlmightyController::FlowRateMaxDlmac(uint32_t payloadBytes, double biOverheadFraction)
{
	NS_LOG_FUNCTION(this);

	payloadBytes =  24000;

	std::vector <double> flowsRate (m_flowsPath.size(), 0.0);

	std::vector <double> staScheduleLenNs (m_meshNodes->GetN(), 0.0);//only care those of txing nodes


	uint32_t overheadTimeEachPayloadNs = 6.45 *1000 +3.0 * 1000;
	//schLen
	for (uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++) 
	{
		//at each relaying node
		for(uint32_t i = 0; i < m_flowsPath[flowIdx].size() - 1; i++)
		{
			uint32_t nextIdx = m_flowsPath[flowIdx][i+1];
			uint32_t staIdx = m_flowsPath[flowIdx][i];

			if(i != 0){
				uint32_t prevIdx = m_flowsPath[flowIdx][i-1];
				Mac48Address staMacAddr = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetAddress();
				WifiMode prev2staWifiMode = m_meshNodes->Get(prevIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()-> GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>()->GetDestinationWifiMode(staMacAddr);

				double phyRateIn = prev2staWifiMode.GetDataRate()/1e6;
				staScheduleLenNs.at(staIdx) += std::ceil(payloadBytes *8 *(1.0/phyRateIn) *1e3) + overheadTimeEachPayloadNs;
				
			}


			Mac48Address nextMacAddr = m_meshNodes->Get(nextIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetAddress();
			WifiMode sta2nextWifiMode = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()-> GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>()->GetDestinationWifiMode(nextMacAddr);

			double phyRateOut = sta2nextWifiMode.GetDataRate()/1e6;
			staScheduleLenNs.at(staIdx) += std::ceil(payloadBytes *8 *(1.0/phyRateOut) *1e3) + overheadTimeEachPayloadNs;

			//NS_LOG_INFO("Flow "<< flowIdx << " between "<< staIdx << " and " << nextIdx << " time " << std::ceil(payloadBytes *8 *(1.0/phyRateOut) *1e3) + overheadTimeEachPayloadNs << " total time at "<< staIdx << " is "<< staScheduleLenNs.at(staIdx) << "ns");

			if(i == m_flowsPath[flowIdx].size() - 2)
			{
				staScheduleLenNs.at(nextIdx) += std::ceil(payloadBytes *8 *(1.0/phyRateOut) *1e3) + overheadTimeEachPayloadNs;
				//NS_LOG_INFO(" total time at "<< nextIdx << " is "<< staScheduleLenNs.at(nextIdx) << "ns");
			}
		}
	}

	//equivalent rate at each node      
	for(uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++)
	{
		double rateAtNode = 0;
		double minRate = 0;

		for(uint32_t i = 0; i < m_flowsPath[flowIdx].size(); i++)
		{
			rateAtNode = payloadBytes *8/staScheduleLenNs.at(m_flowsPath[flowIdx][i])*1e3;

			if (minRate == 0)
				minRate= rateAtNode;
			else
				minRate = (minRate > rateAtNode)?(rateAtNode):(minRate);
		}
		flowsRate.at(flowIdx) = minRate * (1.0 - biOverheadFraction) ;
		NS_LOG_INFO("Flow "<< flowIdx << " estimated rate " << flowsRate.at(flowIdx) << " Mbps");
	}

	return flowsRate;
}


	void
DmgAlmightyController::CreateBlockAckAgreement(void)
{
	for (uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++) {
		for(uint32_t i = 0; i < m_flowsPath[flowIdx].size() - 1; i++){

			Ptr<DmgWifiMac> staMac =
				m_meshNodes->Get(m_flowsPath[flowIdx][i])->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();

			PointerValue ptrStaEdca;
			staMac->GetAttribute ("BE_EdcaTxopN", ptrStaEdca);
			Ptr<EdcaTxopN> staEdca = ptrStaEdca.Get<EdcaTxopN> ();

			Ptr<DmgWifiMac> nextHopMac =
				m_meshNodes->Get(m_flowsPath[flowIdx][i+1])->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();

			PointerValue ptrNextHopEdca;
			nextHopMac->GetAttribute ("BE_EdcaTxopN", ptrNextHopEdca);
			Ptr<EdcaTxopN> nextHopEdca = ptrNextHopEdca.Get<EdcaTxopN> ();

			// We assume immediate block ack with 0 timeout
			nextHopEdca->ForceOriginatorBlockAckAgreement(staMac->GetAddress(), AC_BE,
					0, true);
			staEdca->ForceOriginatorBlockAckAgreement(nextHopMac->GetAddress(), AC_BE,
					0, true);
		}
	}
}

	void
DmgAlmightyController::PrintIdealRxPowerAndMcs(void)
{
	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
		for (uint32_t neighIdx= 0; neighIdx< m_neighbourNodes[staIdx].size(); neighIdx++){
			uint32_t neighId = m_neighbourNodes[staIdx][neighIdx];

			Ptr<DmgWifiMac> neiMac =
				m_meshNodes->Get(neighId)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();

			double neiRxPower = GetIdealRxPower(m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
			double staRxPower = GetIdealRxPower(m_meshNodes->Get(neighId), m_meshNodes->Get(staIdx));

			WifiMode staWifiMode = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()-> GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>()->GetDestinationWifiMode(neiMac->GetAddress());

			NS_LOG_INFO( "Mesh node " << staIdx << " to node " << neighId <<": rx power "
					<< neiRxPower << "dBi; " << staWifiMode.GetUniqueName() <<". rate " << staWifiMode.GetDataRate()/1000000 << "Mbps" );

			if(staWifiMode.GetUniqueName() == "VHTMCS0")
			{
				NS_LOG_INFO( "ERROR: Mesh node "<< staIdx << " is in mode MCS 0." );
				exit(-1);
			}
		}
	}

	//print to file the ideal rate, for matlab DLMAC code

	AsciiTraceHelper ascii;

        std::ostringstream fileName_oss;
			fileName_oss << m_writePath<< "/idealRate_"
				<< Simulator::Now().GetMilliSeconds()/1000.0
				<< "s.txt";

	Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (fileName_oss.str());

	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
		for (uint32_t neighId = 0; neighId < m_meshNodes->GetN(); neighId++){

			if (staIdx == neighId || (std::find(m_neighbourNodes.at(staIdx).begin(),m_neighbourNodes.at(staIdx).end(),neighId) == m_neighbourNodes.at(staIdx).end())){
				*stream->GetStream() <<"0\t";
				continue;
			}

			Ptr<DmgWifiMac> neiMac =
				m_meshNodes->Get(neighId)->GetDevice(0)->GetObject<WifiNetDevice>()->
				GetMac()->GetObject<DmgWifiMac>();

			double neiRxPower = GetIdealRxPower(m_meshNodes->Get(staIdx), m_meshNodes->Get(neighId));
			double staRxPower = GetIdealRxPower(m_meshNodes->Get(neighId), m_meshNodes->Get(staIdx));

			WifiMode staWifiMode = m_meshNodes->Get(staIdx)->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()-> GetWifiRemoteStationManager()->GetObject<DmgDestinationFixedWifiManager>()->GetDestinationWifiMode(neiMac->GetAddress());

			*stream->GetStream() << staWifiMode.GetDataRate() <<"\t";                        

		}
		*stream->GetStream() << std::endl; 
	}
}

	void
DmgAlmightyController::PrintSpInfo(void)
{
	AsciiTraceHelper ascii;

        std::ostringstream timeFileName_oss;
			timeFileName_oss << m_writePath<< "timeAlloc_"
				<< Simulator::Now().GetMilliSeconds()/1000.0
				<< "s.txt";

        NS_LOG_INFO("SP info printed to file " << timeFileName_oss.str());
        Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream (timeFileName_oss.str());

	for (uint32_t staIdx = 0; staIdx < m_meshNodes->GetN(); staIdx++) {
		Ptr<DmgBeaconInterval> dmgBiSta = m_meshNodes->Get(staIdx)->GetDevice(0)->
			GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->GetDmgBeaconInterval();
		uint32_t destinationId;
		//Print sp info
		*stream->GetStream() << "STA "<< staIdx<< " (SP size "<< dmgBiSta->GetSps().size()<<")" << std::endl;
		for (uint32_t spIdx = 0; spIdx < dmgBiSta->GetSps().size(); spIdx++) {
			for (uint32_t i = 0; i < m_meshNodes->GetN(); i++){
				Mac48Address macAdd = m_meshNodes->Get(i)->GetDevice(0)->
					GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac>()->
					GetAddress();
				if (macAdd == dmgBiSta->GetSps().at(spIdx)->GetSpDestination()){
					destinationId = i;
				}
			}

        		*stream->GetStream() << staIdx<< " peer "<< destinationId <<" from " << dmgBiSta->GetSps().at(spIdx)->GetSpStart()<< " to " << dmgBiSta->GetSps().at(spIdx)->GetSpStop() << " Tx? "<< dmgBiSta->GetSps().at(spIdx)->GetSpIfTx() << " final dest to "<< dmgBiSta->GetSps().at(spIdx)->GetSpFlowSourceSinkIpv4Address().second << std::endl;        
		}
	}
}


	void 
DmgAlmightyController::StartMacEnqueueRateMeasurement (void)
{ 
	NS_LOG_FUNCTION(this);
	for (uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++) {
		Ipv4Address ipSink = m_meshNodes->Get(m_flowsPath.at(flowIdx).back())->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
		m_meshNodes->Get(m_flowsPath.at(flowIdx).front())->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac> ()->InstallEnqueueRateMeter (ipSink);
	}
}

	void 
DmgAlmightyController::RestartMacEnqueueRateMeasurement (void)
{ 
	for (uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++) {
		Ipv4Address ipSink = m_meshNodes->Get(m_flowsPath.at(flowIdx).back())->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
		m_meshNodes->Get(m_flowsPath.at(flowIdx).front())->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac> ()->ResetEnqueueRateMeter (ipSink);
	}
}


	std::vector <double>
DmgAlmightyController::GetMacEnqueueRateMeasurement (void)
{
	m_flowDemandMeasured.clear();
	for (uint32_t flowIdx = 0; flowIdx < m_flowsPath.size(); flowIdx++) {
		Ipv4Address ipSink = m_meshNodes->Get(m_flowsPath.at(flowIdx).back())->GetObject<Ipv4>()->GetAddress(1,0).GetLocal();
		Ptr<DmgWifiMac> mac = m_meshNodes->Get(m_flowsPath.at(flowIdx).front())->GetDevice(0)->GetObject<WifiNetDevice>()->GetMac()->GetObject<DmgWifiMac> ();
		m_flowDemandMeasured.push_back(mac-> CalcEnqueueRate (ipSink));

		NS_LOG_UNCOND("FLOW "<< flowIdx <<" measured demand " << m_flowDemandMeasured.at(flowIdx));
	}
	return m_flowDemandMeasured;
}


	void
DmgAlmightyController::ConfigureAntennaAlignment (void)
{
	NS_LOG_FUNCTION(this);
	for (uint32_t aSta = 0; aSta < m_meshNodes->GetN(); aSta++) 
	{
		for (uint32_t bSta = 0; bSta < m_meshNodes->GetN(); bSta++) 
		{
			if(aSta == bSta)
				continue;

			Ptr<YansWifiPhy> aPhy = m_meshNodes->Get(aSta)->GetDevice(0)->GetObject<WifiNetDevice>()->GetPhy()->GetObject<YansWifiPhy> ();
			Ptr<YansWifiPhy> bPhy = m_meshNodes->Get(bSta)->GetDevice(0)->GetObject<WifiNetDevice>()->GetPhy()->GetObject<YansWifiPhy> ();

			Ptr<MobilityModel> aMob = m_meshNodes->Get(aSta)->GetObject<MobilityModel> ();
			Ptr<MobilityModel> bMob = m_meshNodes->Get(bSta)->GetObject<MobilityModel> ();

            aPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,true);
            bPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,true);
          
            if(!m_sim_interference)
            {
                if(std::find(m_neighbourNodes.at(aSta).begin(),m_neighbourNodes.at(aSta).end(),bSta) != m_neighbourNodes.at(aSta).end())
                {
                    //NS_LOG_INFO("LOS between " << aSta << " and "<< bSta);
                    aPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,true);
                    bPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,true);
                }
                else
                {
                    aPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,false);
                    aPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(bMob,aMob,false);
                    bPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(aMob,bMob,false);
                    bPhy->GetChannel()->GetObject<YansWifiChannel>()->GetPropagationLossModel()->GetObject<FriisLoSPropagationLossModel>()->SetLoS(bMob,aMob,false);
                }
            }
		}
	}

}




}
// namespace ns3
