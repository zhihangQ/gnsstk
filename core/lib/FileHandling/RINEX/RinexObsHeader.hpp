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

/**
 * @file RinexObsHeader.hpp
 * Encapsulate header of Rinex observation file, including I/O
 */

#ifndef GNSSTK_RINEXOBSHEADER_HPP
#define GNSSTK_RINEXOBSHEADER_HPP

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <iomanip>

#include "gnsstk_export.h"
#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexObsBase.hpp"
#include "Triple.hpp"
#include "RinexSatID.hpp"
#include "gnsstk_export.h"

namespace gnsstk
{
      /// @ingroup FileHandling
      //@{

      /// RINEX Observation Types
   struct RinexObsType
   {
      std::string type;          ///< 2- char type e.g. L1, P2
      std::string description;   ///< 20- char description (optional) e.g. "L1 pseudorange"
      std::string units;         ///< 10- char units (optional) e.g. "meters"
      unsigned int depend;
      RinexObsType() : type(std::string("UN")),description(std::string("Unknown or Invalid")),
                       units(std::string("")),depend(0) {}
      RinexObsType(std::string t, std::string d, std::string u, unsigned int dep=0) :
            type(t),description(d),units(u),depend(dep) {}
      GNSSTK_EXPORT static const unsigned int C1depend;
      GNSSTK_EXPORT static const unsigned int L1depend;
      GNSSTK_EXPORT static const unsigned int L2depend;
      GNSSTK_EXPORT static const unsigned int P1depend;
      GNSSTK_EXPORT static const unsigned int P2depend;
      GNSSTK_EXPORT static const unsigned int EPdepend;
      GNSSTK_EXPORT static const unsigned int PSdepend;
   };


// Guards are here since an identical struct is used in RinexObsHeader
// and Rinex3NavHeader
#ifndef GNSSTK_EXTRAWAVEFACT
#define GNSSTK_EXTRAWAVEFACT
      /// RINEX 2 extra "WAVELENGTH FACT" lines
   struct ExtraWaveFact
   {
         /// List of Sats with this wavelength factor
      std::vector<SatID> satList;
         /// vector of wavelength factor values
      short wavelengthFactor[2];
   };
#endif  // GNSSTK_EXTRAWAVEFACT


      /**
       * This class models the header for a RINEX Observation File.
       * @sa gnsstk::RinexObsData and gnsstk::RinexObsStream.
       * @sa rinex_obs_test.cpp and rinex_obs_read_write.cpp for examples.
       */
   class RinexObsHeader : public RinexObsBase
   {
   public:

         /// A Simple Constructor.
      RinexObsHeader()
            : version(2.11), interval(0.0), receiverOffset(0), leapSeconds(0),
              numSVs(0), valid(0), numObs(0)
      {
         clear();
      }

         /// Clear (empty out) header
      inline void clear()
      {
//         version = 2.11;
         valid = 0;
         commentList.clear();
         wavelengthFactor[0] = wavelengthFactor[1] = 1;
         extraWaveFactList.clear();
         obsTypeList.clear();
         numObsForSat.clear();
         numObs = 0;
         lastPRN.id = -1;
      }

         /// @name RINEX Observation Header Formatting Strings
         //@{
      GNSSTK_EXPORT
      static const std::string versionString;         ///< "RINEX VERSION / TYPE"
      GNSSTK_EXPORT
      static const std::string runByString;           ///< "PGM / RUN BY / DATE"
      GNSSTK_EXPORT
      static const std::string commentString;         ///< "COMMENT"
      GNSSTK_EXPORT
      static const std::string markerNameString;      ///< "MARKER NAME"
      GNSSTK_EXPORT
      static const std::string markerNumberString;    ///< "MARKER NUMBER"
      GNSSTK_EXPORT
      static const std::string observerString;        ///< "OBSERVER / AGENCY"
      GNSSTK_EXPORT
      static const std::string receiverString;        ///< "REC # / TYPE / VERS"
      GNSSTK_EXPORT
      static const std::string antennaTypeString;     ///< "ANT # / TYPE"
      GNSSTK_EXPORT
      static const std::string antennaPositionString; ///< "APPROX POSITION XYZ"
      GNSSTK_EXPORT
      static const std::string antennaOffsetString;   ///< "ANTENNA: DELTA H/E/N"
      GNSSTK_EXPORT
      static const std::string waveFactString;        ///< "WAVELENGTH FACT L1/2"
      GNSSTK_EXPORT
      static const std::string numObsString;          ///< "# / TYPES OF OBSERV"
      GNSSTK_EXPORT
      static const std::string intervalString;        ///< "INTERVAL"
      GNSSTK_EXPORT
      static const std::string firstTimeString;       ///< "TIME OF FIRST OBS"
      GNSSTK_EXPORT
      static const std::string lastTimeString;        ///< "TIME OF LAST OBS"
      GNSSTK_EXPORT
      static const std::string receiverOffsetString;  ///< "RCV CLOCK OFFS APPL"
      GNSSTK_EXPORT
      static const std::string leapSecondsString;     ///< "LEAP SECONDS"
      GNSSTK_EXPORT
      static const std::string numSatsString;         ///< "# OF SATELLITES"
      GNSSTK_EXPORT
      static const std::string prnObsString;          ///< "PRN / # OF OBS"
      GNSSTK_EXPORT
      static const std::string endOfHeader;           ///< "END OF HEADER"
         //@}

         /// Validity bits for the RINEX Observation Header
      enum validBits
      {
         versionValid         = 0x01,      ///< "RINEX VERSION / TYPE"
         runByValid           = 0x02,      ///< "PGM / RUN BY / DATE"
         commentValid         = 0x04,      ///< "COMMENT"               (optional)
         markerNameValid      = 0x08,      ///< "MARKER NAME"
         markerNumberValid    = 0x010,     ///< "MARKER NUMBER"         (optional)
         observerValid        = 0x020,     ///< "OBSERVER / AGENCY"
         receiverValid        = 0x040,     ///< "REC # / TYPE / VERS"
         antennaTypeValid     = 0x080,     ///< "ANT # / TYPE"
         antennaPositionValid = 0x0100,    ///< "APPROX POSITION XYZ"
         antennaOffsetValid   = 0x0200,    ///< "ANTENNA: DELTA H/E/N"
         waveFactValid        = 0x0400,    ///< "WAVELENGTH FACT L1/2"
         obsTypeValid         = 0x0800,    ///< "# / TYPES OF OBSERV"
         intervalValid        = 0x01000,   ///< "INTERVAL"              (optional)
         firstTimeValid       = 0x02000,   ///< "TIME OF FIRST OBS"
         lastTimeValid        = 0x04000,   ///< "TIME OF LAST OBS"      (optional)
         receiverOffsetValid  = 0x08000,   ///< "RCV CLOCK OFFS APPL"   (optional)
         leapSecondsValid     = 0x0100000, ///< "LEAP SECONDS"          (optional)
         numSatsValid         = 0x0200000, ///< "# OF SATELLITES"       (optional)
         prnObsValid          = 0x0400000, ///< "PRN / # OF OBS"        (optional)

         endValid = 0x080000000,        ///< "END OF HEADER"

            /// This mask is for all required valid fields for RINEX 2.0
         allValid20 = 0x080002FEB,
            /// This mask is for all required valid fields for RINEX 2.1
         allValid21 = 0x080002FEB,
            /// This mask is for all required valid fields for RINEX 2.11
         allValid211 = 0x080002BEB
      };

         /// @name Standard RINEX observation types
         //@{
      GNSSTK_EXPORT static const RinexObsType UN;
      GNSSTK_EXPORT static const RinexObsType L1;
      GNSSTK_EXPORT static const RinexObsType L2;
      GNSSTK_EXPORT static const RinexObsType C1;
      GNSSTK_EXPORT static const RinexObsType C2;
      GNSSTK_EXPORT static const RinexObsType P1;
      GNSSTK_EXPORT static const RinexObsType P2;
      GNSSTK_EXPORT static const RinexObsType D1;
      GNSSTK_EXPORT static const RinexObsType D2;
      GNSSTK_EXPORT static const RinexObsType S1;
      GNSSTK_EXPORT static const RinexObsType S2;
      GNSSTK_EXPORT static const RinexObsType T1;
      GNSSTK_EXPORT static const RinexObsType T2;
      GNSSTK_EXPORT static const RinexObsType C5;
      GNSSTK_EXPORT static const RinexObsType L5;
      GNSSTK_EXPORT static const RinexObsType D5;
      GNSSTK_EXPORT static const RinexObsType S5;
         // Galileo only
      GNSSTK_EXPORT static const RinexObsType C6;
      GNSSTK_EXPORT static const RinexObsType L6;
      GNSSTK_EXPORT static const RinexObsType D6;
      GNSSTK_EXPORT static const RinexObsType S6;
      GNSSTK_EXPORT static const RinexObsType C7;
      GNSSTK_EXPORT static const RinexObsType L7;
      GNSSTK_EXPORT static const RinexObsType D7;
      GNSSTK_EXPORT static const RinexObsType S7;
      GNSSTK_EXPORT static const RinexObsType C8;
      GNSSTK_EXPORT static const RinexObsType L8;
      GNSSTK_EXPORT static const RinexObsType D8;
      GNSSTK_EXPORT static const RinexObsType S8;

         //@}

      GNSSTK_EXPORT
      static const std::vector<RinexObsType> StandardRinexObsTypes;
      GNSSTK_EXPORT
      static std::vector<RinexObsType> RegisteredRinexObsTypes;


      double version;                        ///< RINEX VERSION & TYPE
      std::string fileType;                  ///< RINEX FILETYPE (Observation Navigation etc)
         //std::string system_str;              ///< The string (for file i/o) giving the RINEX system
      RinexSatID system;                     ///< The RINEX satellite system
      std::string fileProgram,               ///< The program used to generate this file
         fileAgency,                         ///< Who ran the program.
         date;                               ///< When the program was run.
      std::vector<std::string> commentList;  ///< Comments in header (optional)
      std::string markerName,                ///< MARKER NAME
         markerNumber;                       ///< MARKER NUMBER (optional)
      std::string observer,                  ///< OBSERVER : who collected the data
         agency;                             ///< OBSERVER'S AGENCY
      std::string recNo,                     ///< RECEIVER NUMBER
         recType,                            ///< RECEIVER TYPE
         recVers;                            ///< RECEIVER VERSION
      std::string antNo,                     ///< ANTENNA NUMBER
         antType;                            ///< ANTENNA TYPE
      gnsstk::Triple antennaPosition;         ///< APPROXIMATE POSITION XYZ
      gnsstk::Triple antennaOffset;           ///< ANTENNA: DELTA H/E/N
      short wavelengthFactor[2];             ///< default WAVELENGTH FACTORS
      std::vector<ExtraWaveFact> extraWaveFactList; ///< extra (per PRN) WAVELENGTH FACTORS
      std::vector<RinexObsType> obsTypeList; ///< NUMBER & TYPES OF OBSERV
      double interval;                       ///< INTERVAL (optional)
      CommonTime firstObs ;                     ///< TIME OF FIRST OBS
      RinexSatID firstSystem;                ///< RINEX satellite system of FIRST OBS timetag
      CommonTime lastObs ;                      ///< TIME OF LAST OBS (optional)
      RinexSatID lastSystem;                 ///< RINEX satellite system of LAST OBS timetag
      int receiverOffset;                    ///< RCV CLOCK OFFS APPL (optional)
      int leapSeconds;                       ///< LEAP SECONDS (optional)
      short numSVs;                          ///< NUMBER OF SATELLITES in following map (optional)
      std::map<SatID, std::vector<int> > numObsForSat; ///<  PRN / # OF OBS (optional)
      unsigned long valid; ///< Bits set when individual header members are present and valid
      int numObs; ///< used to save the number of obs on # / TYPES continuation lines.
      RinexSatID lastPRN; ///< used to save the current PRN while reading PRN/OBS continuation lines.

         /// Destructor
      virtual ~RinexObsHeader() {}

         // The next four lines is our common interface
         /// RinexObsHeader is a "header" so this function always returns true.
      virtual bool isHeader() const {return true;}

         /**
          * This is a simple Debug output function.
          * It simply outputs the version, name and antenna number of this
          * RINEX header.
          */
      virtual void dump(std::ostream& s) const;

         /**
          * This function converts the string in \a oneObs to a RinexObsType.
          * @throw FFStreamError
          */
      static RinexObsType convertObsType(const std::string& oneObs);

         /**
          * This function converts the RinexObsType in \a oneObs to a string.
          * @throw FFStreamError
          */
      static std::string convertObsType(const RinexObsType& oneObs);

         /**
          * Parse a single header record, and modify valid accordingly.
          * Used by reallyGetRecord for both RinexObsHeader and RinexObsData.
          * @throw FFStreamError
          */
      void ParseHeaderRecord(std::string& line);

         /// Compute the number of valid header records which WriteHeaderRecords() will write
      int NumberHeaderRecordsToBeWritten(void) const noexcept;

         /**
          * Write all valid header records to the given stream.
          * Used by reallyPutRecord for both RinexObsHeader and RinexObsData.
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      void WriteHeaderRecords(FFStream& s) const;

         /// Return boolean : is this a valid Rinex header?
      bool isValid() const { return ((valid & allValid20) == allValid20); }

   protected:
         /**
          * outputs this record to the stream correctly formatted.
          * @throw std::exception
          * @throw StringUtils::StringException
          * @throw FFStreamError
          */
      virtual void reallyPutRecord(FFStream& s) const;

         /**
          * This function retrieves the RINEX Header from the given FFStream.
          * If an stream error is encountered, the stream is reset to its
          *  original position and its fail-bit is set.
          * @throw std::exception
          * @throw StringException when a StringUtils function fails
          * @throw FFStreamError when exceptions(failbit) is set and
          *  a read or formatting error occurs.  This also resets the
          *  stream to its pre-read position.
          */
      virtual void reallyGetRecord(FFStream& s);

      friend class RinexObsData;

   private:
         /// Converts the CommonTime \a dt into a Rinex Obs time
         /// string for the header
      std::string writeTime(const CommonTime& dt) const;

         /**
          * This function sets the time for this header.
          * It looks at \a line to obtain the needed information.
          */
      CommonTime parseTime(const std::string& line) const;

   }; // end class RinexObsHeader

      /// operator == for RinexObsType
   inline bool operator==(const RinexObsType& x,
                          const RinexObsType& y) { return (x.type == y.type); }

      /// operator < for RinexObsType
   inline bool operator<(const RinexObsType& x,
                         const RinexObsType& y) { return (x.type < y.type); }

      /// operator << for RinexObsType
   inline std::ostream& operator<<(std::ostream& s,
                                   const RinexObsType rot)
   {
      return s << "Type=" << rot.type
               << ", Description=" << rot.description
               << ", Units=" << rot.units;
   }

      /// Function to allow user to define a new RINEX observation type
   int RegisterExtendedRinexObsType(std::string t,
                                    std::string d=std::string("(undefined)"),
                                    std::string u=std::string("undefined"),
                                    unsigned int dep=0);

      /// Pretty print a list of standard Rinex observation types
   void DisplayStandardRinexObsTypes(std::ostream& s);

      /// Pretty print a list of registered extended Rinex observation types
   void DisplayExtendedRinexObsTypes(std::ostream& s);

      //@}

} // namespace

#endif
