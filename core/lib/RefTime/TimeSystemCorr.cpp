//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

#include "TimeSystemCorr.hpp"
#include "BDSWeekSecond.hpp"

namespace gnsstk
{
   TimeSystemCorrection ::
   TimeSystemCorrection()
         : type(Unknown), frTS(TimeSystem::Unknown), toTS(TimeSystem::Unknown)
   {
      init();
   }


   TimeSystemCorrection ::
   TimeSystemCorrection(std::string str)
   {
      init();
      this->fromString(str);
   }

      // Set members to known (even if invalid) values
   void TimeSystemCorrection::
   init()
   {
      A0 = 0.0;
      A1 = 0.0;
      refTime = CommonTime::BEGINNING_OF_TIME;
      geoProvider = "";
      geoUTCid = 0;
   }

   void TimeSystemCorrection ::
   fromString(const std::string& str)
   {
      std::string STR(gnsstk::StringUtils::upperCase(str));
      if(STR == std::string("GPUT"))
      {
         type = GPUT;
         frTS = TimeSystem::GPS;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("GAUT"))
      {
         type = GAUT;
         frTS = TimeSystem::GAL;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("SBUT"))
            // TD ??
      {
         type = SBUT;
         frTS = TimeSystem::GPS;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("GLUT"))
      {
         type = GLUT;
         frTS = TimeSystem::GLO;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("GPGA") || STR == std::string("GAGP")) // R3.04 A5
      {
         type = GPGA;
         frTS = TimeSystem::GPS;
         toTS = TimeSystem::GAL;
      }
      else if(STR == std::string("GLGP"))
      {
         type = GLGP;
         frTS = TimeSystem::GLO;
         toTS = TimeSystem::GPS;
      }
      else if(STR == std::string("QZGP"))
      {
         type = QZGP;
         frTS = TimeSystem::QZS;
         toTS = TimeSystem::GPS;
      }
      else if(STR == std::string("QZUT"))
      {
         type = QZUT;
         frTS = TimeSystem::QZS;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("BDUT"))
      {
         type = BDUT;
         frTS = TimeSystem::BDT;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("BDGP"))
      {
         type = BDGP;
         frTS = TimeSystem::BDT;
         toTS = TimeSystem::GPS;
      }
      else if(STR == std::string("IRUT"))
      {
         type = IRUT;
         frTS = TimeSystem::IRN;
         toTS = TimeSystem::UTC;
      }
      else if(STR == std::string("IRGP"))
      {
         type = IRGP;
         frTS = TimeSystem::IRN;
         toTS = TimeSystem::GPS;
      }
      else
      {
         Exception e("Unknown TimeSystemCorrection type: " + str);
         GNSSTK_THROW(e);
      }
   }


   bool TimeSystemCorrection ::
   convertTimeSystemToCorrType(TimeSystem src, TimeSystem tgt,
                               TimeSystemCorrection::CorrType& ct)
   {
      ct = TimeSystemCorrection::Unknown;
      switch (src)
      {
         case TimeSystem::BDT:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::BDUT;
                  break;
               case TimeSystem::GPS:
                  ct = TimeSystemCorrection::BDGP;
                  break;
               default:
                  break;
            }
            break;
         case TimeSystem::GAL:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::GAUT;
                  break;
               case TimeSystem::GPS:
                  ct = TimeSystemCorrection::GAGP;
                  break;
               default:
                  break;
            }
            break;
         case TimeSystem::GLO:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::GLUT;
                  break;
               case TimeSystem::GPS:
                  ct = TimeSystemCorrection::GLGP;
                  break;
               default:
                  break;
            }
            break;
         case TimeSystem::GPS:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::GPUT;
                  break;
               case TimeSystem::GAL:
                  ct = TimeSystemCorrection::GPGA;
                  break;
               default:
                  break;
            }
            break;
         case TimeSystem::IRN:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::IRUT;
                  break;
               case TimeSystem::GPS:
                  ct = TimeSystemCorrection::IRGP;
                  break;
               default:
                  break;
            }
            break;
         case TimeSystem::QZS:
            switch (tgt)
            {
               case TimeSystem::UTC:
                  ct = TimeSystemCorrection::QZUT;
                  break;
               case TimeSystem::GPS:
                  ct = TimeSystemCorrection::QZGP;
                  break;
               default:
                  break;
            }
            break;
         default:
            break;
      }
      return (ct != TimeSystemCorrection::Unknown);
   }


   std::string TimeSystemCorrection ::
   asString() const
   {
      switch (type)
      {
         case GPUT: return std::string("GPS to UTC");
         case GAUT: return std::string("GAL to UTC");
         case SBUT: return std::string("SBAS to UTC");
         case GLUT: return std::string("GLO to UTC");
         case GPGA: return std::string("GPS to GAL");
         case GLGP: return std::string("GLO to GPS");
         case QZGP: return std::string("QZS to GPS");
         case QZUT: return std::string("QZS to UTC");
         case BDUT: return std::string("BDT to UTC");
         case BDGP: return std::string("BDT to GPS");
         case IRUT: return std::string("IRN to UTC");
         case IRGP: return std::string("IRN to GPS");
         default:   return std::string("ERROR");
      }
   }


   std::string TimeSystemCorrection ::
   asString4() const
   {
      switch(type) {
         case GPUT: return std::string("GPUT");
         case GAUT: return std::string("GAUT");
         case SBUT: return std::string("SBUT");
         case GLUT: return std::string("GLUT");
         case GPGA: return std::string("GPGA");
         case GLGP: return std::string("GLGP");
         case QZGP: return std::string("QZGP");
         case QZUT: return std::string("QZUT");
         case BDUT: return std::string("BDUT");
         case BDGP: return std::string("BDGP");
         case IRUT: return std::string("IRUT");
         case IRGP: return std::string("IRGP");
         default:   return std::string("ERROR");
      }
   }


   void TimeSystemCorrection ::
   dump(std::ostream& s) const
   {
      s << "Time system correction for " << asString4() << ": "
        << asString() << std::scientific << std::setprecision(12);
      BDSWeekSecond bdt;
      GPSWeekSecond gps;
      CivilTime civ;
      switch(type) {
         case GPUT:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case GAUT:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case SBUT:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow)
              << ", provider " << geoProvider << ", UTC ID = " << geoUTCid;
            break;
         case GLUT:
            gps = refTime;
            s << ", -TauC = " << A0
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case GPGA:
            gps = refTime;
            s << ", A0G = " << A0 << ", A1G = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case GLGP:
            civ = refTime;
            s << ", TauGPS = " << A0 << " sec, RefTime = yr/mon/day "
              << civ.year << "/" << civ.month << "/" << civ.day;
            break;
         case QZGP:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case QZUT:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case BDUT:
            bdt = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << bdt.week << "/" << int(bdt.sow);
            break;
         case BDGP:
            bdt = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << bdt.week << "/" << int(bdt.sow);
            break;
         case IRUT:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
            break;
         case IRGP:
            gps = refTime;
            s << ", A0 = " << A0 << ", A1 = " << A1
              << ", RefTime = week/sow " << gps.week << "/" << int(gps.sow);
         default:
            break;
      }
         //s << std::endl;
   }


   bool TimeSystemCorrection ::
   isConverterFor(const TimeSystem& ts1, const TimeSystem& ts2) const
   {
      if(ts1 == ts2)
      {
         Exception e("Identical time systems");
         GNSSTK_THROW(e);
      }
      if(ts1 == TimeSystem::Unknown || ts2 == TimeSystem::Unknown)
      {
         Exception e("Unknown time systems");
         GNSSTK_THROW(e);
      }
      if((ts1 == frTS && ts2 == toTS) || (ts2 == frTS && ts1 == toTS))
      {
         return true;
      }
      return false;
   }


   double TimeSystemCorrection ::
   Correction(const CommonTime& ct) const
   {
      double corr(0.0), dt;
      TimeSystem fromTS(ct.getTimeSystem());
      CommonTime rtc(refTime); // copy of ref time to allow time system change
      Exception e("Unable to compute correction - wrong TimeSystem");
      Exception eSBAS("TimeSystemCorrection SBAS <=> UTC has not been implemented");
      rtc.setTimeSystem(fromTS);

      switch(type)
      {
         case GPUT:
            if(fromTS != TimeSystem::GPS && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - refTime
            dt = ct - rtc;

            if(fromTS == TimeSystem::GPS)             // GPS => UTC
               corr = -A0-A1*dt;
            else                                      // UTC => GPS
               corr = A0+A1*dt;

            break;

         case GAUT:
            if(fromTS != TimeSystem::GAL && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::GAL)             // GAL => UTC
               corr = -A0-A1*dt;
            else                                      // UTC => GAL
               corr = A0+A1*dt;

            break;

         case SBUT:
            GNSSTK_THROW(eSBAS);
            break;

         case GLUT:
            if(fromTS != TimeSystem::GLO && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

            if(fromTS == TimeSystem::GLO)             // GLO => UTC
               corr = -A0;
            else                                      // UTC => GLO
               corr = A0;

            break;

         case GPGA:
            if(fromTS != TimeSystem::GPS && fromTS != TimeSystem::GAL)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::GPS)             // GPS => GAL
               corr = A0+A1*dt;
            else                                      // GAL => GPS
               corr = -A0-A1*dt;

            break;

         case GLGP:
            if(fromTS != TimeSystem::GLO && fromTS != TimeSystem::GPS)
            {
               GNSSTK_THROW(e);
            }

            if(fromTS == TimeSystem::GLO)             // GLO => GPS
               corr = A0;
            else                                      // GPS => GLO
               corr = -A0;

            break;

         case QZGP:
            if(fromTS != TimeSystem::QZS && fromTS != TimeSystem::GPS)
            {
               GNSSTK_THROW(e);
            }

            if(fromTS == TimeSystem::QZS)             // QZS => GPS
               corr = 0.0;    // TD?
            else                                      // GPS => QZS
               corr = 0.0;    // TD?

            break;

         case QZUT:
            if(fromTS != TimeSystem::QZS && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::QZS)             // QZS => UTC
               corr = -A0-A1*dt;
            else                                      // UTC => QZS
               corr =  A0+A1*dt;

            break;

         case BDUT:
            if(fromTS != TimeSystem::BDT && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::BDT)             // BDT => UTC
               corr = -A0-A1*dt;
            else                                      // UTC => BDT
               corr =  A0+A1*dt;

            break;

         case BDGP:
            if(fromTS != TimeSystem::BDT && fromTS != TimeSystem::GPS)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::BDT)             // BDT => GPS
               corr = A0;
            else                                      // GPS => BDT
               corr = -A0;

            break;

         case IRUT:
            if(fromTS != TimeSystem::IRN && fromTS != TimeSystem::UTC)
            {
               GNSSTK_THROW(e);
            }

               // dt = fromTime - rtc
            dt = ct - rtc;

            if(fromTS == TimeSystem::IRN)             // GPS => UTC
               corr = -A0-A1*dt;
            else                                      // UTC => GPS
               corr = A0+A1*dt;

            break;

         case IRGP:
            if(fromTS != TimeSystem::IRN && fromTS != TimeSystem::GPS)
            {
               GNSSTK_THROW(e);
            }

            dt = ct - rtc;

            if(fromTS == TimeSystem::IRN)             // IRN => GPS
               corr = -A0-A1*dt;
            else                                      // GPS => IRn
               corr = A0+A1*dt;

            break;

         default:
            Exception e("TimeSystemCorrection is not defined.");
            GNSSTK_THROW(e);
            break;
      }

      return corr;
   }


   void TimeSystemCorrection ::
   fixTimeSystem()
   {
      switch (type)
      {
         case GPUT:    ///< GPS  to UTC using A0, A1
            refTime.setTimeSystem(TimeSystem::GPS);
            break;
         case GLUT:    ///< GLO  to UTC using A0 = -TauC , A1 = 0
            refTime.setTimeSystem(TimeSystem::GLO);
            break;
         case GAUT:    ///< GAL  to UTC using A0, A1
            refTime.setTimeSystem(TimeSystem::GAL);
            break;
         case BDUT:    ///< BDT  to UTC using A0, A1
            refTime.setTimeSystem(TimeSystem::BDT);
            break;
         case QZUT:    ///< QZS  to UTC using A0, A1
            refTime.setTimeSystem(TimeSystem::QZS);
            break;
         case IRUT:    ///< IRN  to UTC using A0, A1
            refTime.setTimeSystem(TimeSystem::IRN);
            break;
         case SBUT:    ///< SBAS to UTC using A0, A1, incl. provider and UTC ID
            refTime.setTimeSystem(TimeSystem::Unknown);
            break;
         case GLGP:    ///< GLO  to GPS using A0 = TauGPS, A1 = 0
            refTime.setTimeSystem(TimeSystem::GLO);
            break;
         case GAGP:    ///< GAL  to GPS using A0 = A0G   , A1 = A1G
            refTime.setTimeSystem(TimeSystem::GAL);
            break;
         case QZGP:    ///< QZS  to GPS using A0, A1
            refTime.setTimeSystem(TimeSystem::QZS);
            break;
         case IRGP:     ///< IRN  to GPS using A0, A1
            refTime.setTimeSystem(TimeSystem::IRN);
            break;
         case GPGA:    ///< GPS  to GAL using A0 = A0G   , A1 = A1G, RINEX 3.03
            refTime.setTimeSystem(TimeSystem::GPS);
            break;
         case BDGP:    ///< BDT  to GPS using A0, A1  !! not in RINEX
            refTime.setTimeSystem(TimeSystem::BDT);
            break;
         default:
            refTime.setTimeSystem(TimeSystem::Unknown);
            break;
      }
   }

} // namespace gnsstk
