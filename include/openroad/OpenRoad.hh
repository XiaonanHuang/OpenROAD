// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef OPENROAD_H
#define OPENROAD_H

namespace odb {
class dbDatabase;
}

namespace sta {
class StaDb;
class DbNetwork;
}

namespace ord {

// Lightweight version of OpenRoad app for stand-alone version.
// Only pointers to components so the header has no dependents.
class OpenRoad
{
public:
  OpenRoad();
  ~OpenRoad();
  // Singleton accessor used by tcl command interpreter.
  static OpenRoad *openRoad() { return &open_road_; }
  odb::dbDatabase *getDb() { return db_; }
  sta::StaDb *getSta() { return sta_; }
  sta::DbNetwork *getDbNetwork();
  void readLef(const char *filename,
	       const char *lib_name,
	       bool make_tech,
	       bool make_library);
  void readDef(const char *filename);
  void writeDef(const char *filename);
  void readDb(const char *filename);
  void writeDb(const char *filename);

private:
  odb::dbDatabase *db_;
  sta::StaDb *sta_;

  // Singleton used by tcl command interpreter.
  static OpenRoad open_road_;
};

} // namespace

#endif