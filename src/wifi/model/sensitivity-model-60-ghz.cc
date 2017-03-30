/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "ns3/log.h"
#include "ns3/interference-helper.h"

#include "wifi-phy.h"
#include "sensitivity-model-60-ghz.h"
#include "sensitivity-lut.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("SensitivityModel60GHz");

NS_OBJECT_ENSURE_REGISTERED(SensitivityModel60GHz);

TypeId SensitivityModel60GHz::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::SensitivityModel60GHz")
        .SetParent<ErrorRateModel> ()
        .AddConstructor<SensitivityModel60GHz>()
        ;
    return tid;
}

SensitivityModel60GHz::SensitivityModel60GHz()
{}

double
SensitivityModel60GHz::CalculateSnr (WifiMode txMode, double ber) const
{
  /* This is a very simple binary search */
  double lowDb, highDb, precisionDb, middle, middleDb;
  /*
   * The "dB" quantities are actually dB/10, but we would just be multiplying
   * and dividing, so skip it
   */
  lowDb = log10(1e-10);
  highDb = log10(1e10);
  precisionDb = 0.01 / 10; /* Accurate to 0.01 dB */
  do
    {
      NS_ASSERT (highDb >= lowDb);
      middleDb = (lowDb + highDb) / 2;
      middle = pow(10,middleDb);
      if ((1 - GetChunkSuccessRate (txMode, middle, 1)) > ber) 
        {
          lowDb = middleDb;
        } 
      else 
        {
          highDb = middleDb;
        }
    } while (highDb - lowDb > precisionDb) ;
  return middle;
}

double SensitivityModel60GHz::GetChunkSuccessRate(WifiMode mode, double snr, uint32_t nbits) const
{
    NS_ASSERT_MSG(mode.GetModulationClass() == WIFI_MOD_CLASS_VHT_CTRL ||
		  mode.GetModulationClass() == WIFI_MOD_CLASS_VHT_SC ||
		  mode.GetModulationClass() == WIFI_MOD_CLASS_VHT_OFDM,
		 "Expecting 802.11ad VHT CTRL, SC or OFDM modulation");
    std::string modename = mode.GetUniqueName();

    /* This is kinda silly, but convert from SNR back to RSS */
    double noise = 1.3803e-23 * 290.0 * mode.GetBandwidth();

    /* Compute RSS in dBm, so add 30 from SNR */
    double rss = 10 * log10(snr * noise) + 30;
    NS_LOG_FUNCTION(modename << "snr" << snr << "rss" << rss << "bits" << nbits);
    double rss_delta;
    double ber;

    /**** Control PHY ****/
    if (modename == "VHTMCS0")
	rss_delta = rss - -78;

    /**** SC PHY ****/
    else if (modename == "VHTMCS1")
        rss_delta = rss - -68;
    else if (modename == "VHTMCS2")
        rss_delta = rss - -67;
    else if (modename == "VHTMCS3")
        rss_delta = rss - -65;
    else if (modename == "VHTMCS4")
        rss_delta = rss - -64;
    else if (modename == "VHTMCS5")
        rss_delta = rss - -62;
    else if (modename == "VHTMCS6")
        rss_delta = rss - -63;
    else if (modename == "VHTMCS7")
        rss_delta = rss - -62;
    else if (modename == "VHTMCS8")
        rss_delta = rss - -61;
    else if (modename == "VHTMCS9")
        rss_delta = rss - -59;
    else if (modename == "VHTMCS10")
        rss_delta = rss - -55;
    else if (modename == "VHTMCS11")
        rss_delta = rss - -54;
    else if (modename == "VHTMCS12")
        rss_delta = rss - -53;

    /**** Extrapolated SC PHY ****/
    else if (modename == "VHTMCS13")
        rss_delta = rss - -52;	/* Basis: rel sens. for OFDM analog */
    else if (modename == "VHTMCS14")
        rss_delta = rss - -50;	/* Basis: rel sens. for OFDM analog */
    else if (modename == "VHTMCS15")
        rss_delta = rss - -48;	/* Basis: rel sens. for OFDM analog */
    else if (modename == "VHTMCS16")
        rss_delta = rss - -46;	/* Basis: rel sens. for OFDM analog */
    else if (modename == "VHTMCS17")
        rss_delta = rss - -42;	/* Basis: 256 is 6 dB worse than 64 */
    else if (modename == "VHTMCS18")
        rss_delta = rss - -40;	/* Basis: 256 is 6 dB worse than 64 */

    /**** OFDM PHY ****/
    else if (modename == "VHTMCS13a")
        rss_delta = rss - -66;
    else if (modename == "VHTMCS14a")
        rss_delta = rss - -64;
    else if (modename == "VHTMCS15a")
        rss_delta = rss - -63;
    else if (modename == "VHTMCS16a")
        rss_delta = rss - -62;
    else if (modename == "VHTMCS17a")
        rss_delta = rss - -60;
    else if (modename == "VHTMCS18a")
        rss_delta = rss - -58;
    else if (modename == "VHTMCS19a")
        rss_delta = rss - -56;
    else if (modename == "VHTMCS20a")
        rss_delta = rss - -54;
    else if (modename == "VHTMCS21a")
        rss_delta = rss - -53;
    else if (modename == "VHTMCS22a")
        rss_delta = rss - -51;
    else if (modename == "VHTMCS23a")
        rss_delta = rss - -49;
    else if (modename == "VHTMCS24a")
        rss_delta = rss - -47;

    /**** Low power PHY ****/
    else if (modename == "VHTMCS25a")
        rss_delta = rss - -64;
    else if (modename == "VHTMCS26a")
        rss_delta = rss - -60;
    else if (modename == "VHTMCS27a")
        rss_delta = rss - -57;
    else
        NS_FATAL_ERROR("Unrecognized 60 GHz modulation");

    /* Compute BER in lookup table */
    if (rss_delta < -12.0)
        ber = sensitivity_ber(0);
    else if (rss_delta > 6.0)
        ber = sensitivity_ber(180);
    else
        ber = sensitivity_ber((int)(10 * (rss_delta + 12)));

    /* Compute PER from BER */
    return pow(1 - ber, nbits);
}

} // namespace ns3
