module MPK
{

   interface Line;
   interface Tram;
   interface Stop;
   interface User;



   sequence<Line*> lines;
   sequence<Stop*> stops;
   sequence<Tram*> trams;



   exception noSuchLine{};
   exception noSuchTram{};
   exception noSuchStop{};

   

   struct TimeOfDay
   {
       short hour;
       short minute;
   };


   interface SIP
   {
      lines getLines();
      void addLine(Line* line);
      void removeLine(Line* line) throws noSuchLine;
      Stop* getStop(int ID) throws noSuchStop;
   }



   struct ArrivalInfoItem
   {
       Tram* tram;
       TimeOfDay time;
   }

   
   sequence<ArrivalInfoItem> arrivals;

   exception noSuchUser{};

   interface Stop
   {
      lines getLines();

      int getID();

      string getName();

      arrivals getArrivals();

      void registerUser(User* user);

      void unregisterUser(User* user) throws noSuchUser;

      void updateUsers();
   }



   interface Line
   {
		trams getTrams();

		stops getStops();

		void addTram(Tram* tram);

		void removeTram(Tram* tram) throws noSuchTram;
   }

   struct ScheduleItem
   {
      Stop* stop;

      TimeOfDay time;
   }

   
   sequence<ScheduleItem> schedule;



   interface Tram
   {
	   Stop* getStop();
       TimeOfDay getStopTime(int stopid);
       int getID();
       schedule getSchedule();   
       void registerUser(User* user);
       void unregisterUser(User* user) throws noSuchUser;
   }

   interface User
   {
      void updateStop(Tram* tram, Stop* stop);

      void updateSchedule(int stopID, string stopName, arrivals arr);
   }

}