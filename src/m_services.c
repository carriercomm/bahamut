/* m_services.c - Because s_user.c was just crazy.
 *   Copyright (C) 1990 Jarkko Oikarinen and
 *                      University of Oulu, Computing Center
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers.
 *
 *   This program is free softwmare; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id$ */

#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "channel.h"
#include <sys/stat.h>
#include <utmp.h>
#include <fcntl.h>
#include "h.h"

/* Externally defined stuffs */
extern int user_modes[];

/* internally defined stuffs */




/*
 * the services aliases. *
 *
 * NICKSERV     - /nickserv * CHANSERV  - /chanserv * OPERSERV  -
 * /operserv * MEMOSERV         - /memoserv * SERVICES  - /services *
 * IDENTIFY     - /identify * taz's code -mjs
 */

int
m_chanserv(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;
   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((acptr = find_person(ChanServ, NULL)))
      sendto_one(acptr, ":%s PRIVMSG %s@%s :%s", parv[0],
                 ChanServ, SERVICES_NAME, parv[1]);
   else
      sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                 parv[0], ChanServ);
   return 0;
}

/*
 * m_nickserv
 */
int
m_nickserv(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;
   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((acptr = find_person(NickServ, NULL)))
      sendto_one(acptr, ":%s PRIVMSG %s@%s :%s", parv[0],
                 NickServ, SERVICES_NAME, parv[1]);
   else
      sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                 parv[0], NickServ);
   return 0;
}
/*
 * m_memoserv
 */
int
m_memoserv(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;
   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((acptr = find_person(MemoServ, NULL)))
      sendto_one(acptr, ":%s PRIVMSG %s@%s :%s", parv[0],
                 MemoServ, SERVICES_NAME, parv[1]);
   else
      sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                 parv[0], MemoServ);
   return 0;
}

/*
 * m_operserv
 */
int
m_operserv(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;
   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((acptr = find_person(OperServ, NULL)))
      sendto_one(acptr, ":%s PRIVMSG %s@%s :%s", parv[0],
                OperServ,
#ifdef OPERSERV_OTHER_HOST
                OPERSERV_OTHER_HOST,
#else
                SERVICES_NAME,
#endif
                parv[1]);

   else
      sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                 parv[0], OperServ);
   return 0;
}
/*
 * m_statserv
 */
int
m_statserv(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;
   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((acptr = find_person(StatServ, NULL)))
      sendto_one(acptr, ":%s PRIVMSG %s@%s :%s", parv[0],
                StatServ,
#ifdef OPERSERV_OTHER_HOST
                OPERSERV_OTHER_HOST,
#else
                SERVICES_NAME,
#endif
                parv[1]);

   else
      sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                 parv[0], OperServ);
   return 0;
}

/*
 * m_services -- see df465+taz
 */
int
m_services(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
char       *tmps;

   if (check_registered_user(sptr))
      return 0;

   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if ((strlen(parv[1]) >= 4) && (!myncmp(parv[1], "help", 4))) {
      sendto_one(sptr, ":services!service@%s NOTICE %s :For ChanServ "
                 "help use: /chanserv help", SERVICES_NAME,
                 sptr->name);
      sendto_one(sptr, ":services!service@%s NOTICE %s :For NickServ "
                 "help use: /nickserv help", SERVICES_NAME,
                 sptr->name);
      sendto_one(sptr, ":services!service@%s NOTICE %s :For MemoServ "
                 "help use: /memoserv help", SERVICES_NAME,
                 sptr->name);
      return 0;
   }
   if ((tmps = (char *) strchr(parv[1], ' '))) {
      for(; *tmps == ' '; tmps++); /* er.. before this for loop, the next
                * comparison would always compare '#' with ' '.. oops. - lucas
                */
      if (*tmps == '#')
         return m_chanserv(cptr, sptr, parc, parv);
      else
         return m_nickserv(cptr, sptr, parc, parv);
   }
   return m_nickserv(cptr, sptr, parc, parv);
}

/*
 * m_identify  df465+taz
 */
int
m_identify(cptr, sptr, parc, parv)
     aClient    *cptr, *sptr;
     int         parc;
     char       *parv[];
{
aClient    *acptr;

   if (check_registered_user(sptr))
      return 0;

   if (parc < 2 || *parv[1] == '\0') {
      sendto_one(sptr, err_str(ERR_NOTEXTTOSEND), me.name, parv[0]);
      return -1;
   }
   if (*parv[1]) {
      if ((*parv[1] == '#') && ((char *) strchr(parv[1], ' '))) {
         if ((acptr = find_person(ChanServ, NULL)))
            sendto_one(acptr, ":%s PRIVMSG %s@%s :IDENTIFY %s "
                       ,parv[0], ChanServ,
                       SERVICES_NAME, parv[1]);
         else
            sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name, parv[0], ChanServ);
      }
      else {
         if ((acptr = find_person(NickServ, NULL)))
            sendto_one(acptr, ":%s PRIVMSG %s@%s :IDENTIFY %s", parv[0],
                       NickServ, SERVICES_NAME, parv[1]);
         else
            sendto_one(sptr, err_str(ERR_SERVICESDOWN), me.name,
                       parv[0], NickServ);
      }
   }
   return 0;
}

/* s_svsnick - Pretty straight forward.  Mostly straight outta df
 *  - Raistlin
 * parv[0] = sender
 * parv[1] = old nickname
 * parv[2] = new nickname
 * parv[3] = timestamp
 */
int m_svsnick(aClient *cptr, aClient *sptr, int parc, char *parv[]) {
        aClient *acptr;

        if (!IsULine(sptr)||parc < 4||(strlen(parv[2]) > NICKLEN)) return 0;
        /* if we can't SVSNICK them to something because the nick is in use, KILL them */
        acptr=find_person(parv[2], NULL);
        if(acptr!=NULL) {
                /* send a kill out for the person instead --wd */
                sendto_serv_butone(cptr, ":%s KILL %s :%s (SVSNICK Collide)",
                                                                 sptr->name, parv[1], sptr->name);
                /* now send back a kill for the nick they were presumably changed to */
                sendto_one(cptr, ":%s KILL %s :%s (SVSNICK Collide)",
                                          sptr->name, parv[2], sptr->name);
                return 0;
        }
        if (!hunt_server(cptr, sptr, ":%s SVSNICK %s %s :%s", 1, parc, parv) != HUNTED_ISME) {
                        if ((acptr = find_person(parv[1], NULL))!=NULL) {
                                 acptr->umode &= ~UMODE_r;
                                 acptr->last_nick_change = atoi(parv[3]);
                                 sendto_common_channels(acptr, ":%s NICK :%s", parv[1], parv[2]);
                                 if (IsPerson(acptr)) add_history(acptr, 1);
                                 sendto_serv_butone(NULL, ":%s NICK %s :%i", parv[1], parv[2], atoi(parv[3]));
                                 if(acptr->name[0]) del_from_client_hash_table(acptr->name, acptr);
                                 strcpy(acptr->name, parv[2]);
                                 add_to_client_hash_table(parv[2], acptr);
                        }
         }
        return 0;
}

/* channel_svsmode:
 * parv[0] sender
 * parv[1] channel
 * parv[2] modes
 * parv[3] nick
 * parv[4] nickts
 * currently, only a mode of -b is supported.
 * services should use MODE for regular channel modes.
 * 2/5/00 lucas
 * preconditions: parc >= 3, sptr is ulined
 */
int channel_svsmode(aClient *cptr, aClient *sptr, int parc, char *parv[])
{
   aChannel *chptr;
   aClient *acptr = NULL;
   char *m, *nick = NULL;
   char change = '+';
   ts_val nickts = 0;
   int sendmsg = 1;

   if(!(chptr = find_channel(parv[1], NULL)))
      return 0;

   if(parc >= 4)
   {
      nick = parv[3];
      if(parc > 4)
         nickts = atol(parv[4]);
   }

   if(nick)
   {
      acptr = find_person(nick, NULL);
      if(!acptr || (nickts && acptr->tsinfo != nickts))
         return 0;
   }

   for(m = parv[2]; *m; m++)
      switch(*m)
      {
         case '+':
         case '-':
            change = *m;
            break;

         case 'b':
            if(nick && MyClient(acptr) && change == '-')
            {
               remove_matching_bans(chptr, acptr, &me);
               sendmsg--;
            }
            break;

         default:
            sendmsg++;
            break;
      }

   if(!sendmsg) return 0;

   if(nick)
      sendto_serv_butone(cptr, ":%s SVSMODE %s %s %s %ld", parv[0], parv[1], parv[2], nick, acptr->tsinfo);
   else
      sendto_serv_butone(cptr, ":%s SVSMODE %s %s", parv[0], parv[1], parv[2]);

   return 0;
}

/* m_svsmode - df function integrated
 *  - Raistlin
 * -- Behaviour changed - Epi (11/30/99)
 * parv[0] - sender
 * parv[1] - nick
 * parv[2] - TS (or mode, depending on svs version)
 * parv[3] - mode (or services id if old svs version)
 * parv[4] - optional arguement (services id)
 */
int m_svsmode(aClient *cptr, aClient *sptr, int parc, char *parv[]) {
         int            flag, *s, what, setflags;
         char          *m, *modes, *optarg;
         aClient       *acptr;
         ts_val         ts = 0;

         if (!IsULine(sptr) || (parc < 3))
                return 0;

         if (parv[1][0] == '#')
            return channel_svsmode(cptr, sptr, parc, parv);

         if ((parc >= 4) && ((parv[3][0] == '+') || (parv[3][0] == '-')))
         {
                ts = atol(parv[2]);
                modes = parv[3];
                optarg = (parc > 4) ? parv[4] : NULL;
         }
         else
         {
                modes = parv[2];
                optarg = (parc > 3) ? parv[3] : NULL;
         }

         if (!(acptr = find_person(parv[1], NULL)))
                return 0;

         if (ts && (ts != acptr->tsinfo))
                return 0;

         what = MODE_ADD;
         setflags = 0;

         for (s = user_modes; (flag = *s); s += 2)
                 if (acptr->umode & flag)
                         setflags |= flag;
         for (m = modes; *m; m++)
            switch(*m) {
                 case '+':
                        what = MODE_ADD;
                        break;
                 case '-':
                        what = MODE_DEL;
                        break;
                 case ' ':
                 case '\n':
                 case '\r':
                 case '\t':
                        break;
/* We don't do this yet (i broke this, and am too lazy to fix it -epi)
 *               case 'l':
 *                      if(parv[3] && isdigit(*parv[4])) max_global_count = atoi(parv[3]);
 *                      break;
 */
                 case 'd':
                        if (optarg && isdigit(*optarg))
                           acptr->user->servicestamp = strtoul(optarg, NULL, 0);
                        break;
                 default:
                        for (s = user_modes; (flag = *s); s += 2)
                        {
                           if (*m == (char)(*(s+1)))
                           {
                              if (what == MODE_ADD)
                                 acptr->umode |= flag;
                              else
                                 acptr->umode &= ~flag;
                              break;
                           }
                        }
                        break;
         }

         if (optarg)
                sendto_serv_butone(cptr, ":%s SVSMODE %s %ld %s %s",
                        parv[0], parv[1], acptr->tsinfo, modes, optarg);
         else
                sendto_serv_butone(cptr, ":%s SVSMODE %s %ld %s",
                        parv[0], parv[1], acptr->tsinfo, modes);

         return 0;
}
