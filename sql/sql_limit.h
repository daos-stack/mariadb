/*  Copyright (c) 2019, MariaDB Corporation.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1335  USA */


#ifndef INCLUDES_MARIADB_SQL_LIMIT_H
#define INCLUDES_MARIADB_SQL_LIMIT_H
/**
  LIMIT/OFFSET parameters for execution.
*/

class Select_limit_counters
{
  ha_rows select_limit_cnt, offset_limit_cnt;
  bool with_ties;

  public:
    Select_limit_counters():
       select_limit_cnt(0), offset_limit_cnt(0), with_ties(false)
       {};
    Select_limit_counters(Select_limit_counters &orig):
       select_limit_cnt(orig.select_limit_cnt),
       offset_limit_cnt(orig.offset_limit_cnt),
       with_ties(orig.with_ties)
       {};

   void set_limit(ha_rows limit, ha_rows offset, bool with_ties)
   {
      offset_limit_cnt= offset;
      select_limit_cnt= limit;
      this->with_ties= with_ties;
      if (select_limit_cnt + offset_limit_cnt >=
          select_limit_cnt)
        select_limit_cnt+= offset_limit_cnt;
      else
        select_limit_cnt= HA_POS_ERROR;
   }

   void set_single_row()
   {
     offset_limit_cnt= 0;
     select_limit_cnt= 1;
   }

   bool is_unlimited() const
   { return select_limit_cnt == HA_POS_ERROR; }
   void set_unlimited()
   { select_limit_cnt= HA_POS_ERROR; offset_limit_cnt= 0; }

   bool check_offset(ha_rows sent) const
   {
     return sent < offset_limit_cnt;
   }
   void remove_offset() { offset_limit_cnt= 0; }

   ha_rows get_select_limit() const
   { return select_limit_cnt; }
   ha_rows get_offset_limit() const
   { return offset_limit_cnt; }
   bool is_with_ties() const
   { return with_ties; }
};

#endif // INCLUDES_MARIADB_SQL_LIMIT_H
