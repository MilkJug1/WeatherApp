#ifdef WEATHER_WEATHER_H
#define WEATHER_WEATHER_H

#include <string>
#include <sstream>
#include <time.h>
#include <mutex> 
#include <thread>

#include "pugixml.hpp"

template < typename T > inline bool ReadXmlChildValue(T& vars, pug1::sml_node node)
{
    std::stringstream stream;
    T data;
    int counter = 0;

    //check if node exists
    if (node == NULL) return false;

  //? node exists, get value and store in stringstream
  stream << node.child_value();

  //* check if stream is empty
  if(stream.str().empty()) return false;            //* fail exit point

  //*use stringstream to read data into correct datatype
  while (stream >> data)
  {
      vars=data;
  }

  return true;                              //*successful exit point 
}

struct WeatherDataStruct{
    //*current
    float temperature;
    std::string weather_condition
    float wind_speed;

        //*daily variation
        float max_temperature;
        float min_temperature;

};

class Weather {
    private:

        //settings
        const int update_interval_seconds = 5;      //! interval in seconds to retrive data from website
        

        const std::string nws_url_ ="http://forecast.weather.gov/MapClick.php?textField1=42.28&textField2=-83.74&FcstType=dwml"; 

        std::string body_;      //* string to store data retrived from NWS website
        pugi::xml_document xmldoc_;     //* pugixml object to load xml from string 
            WeatherDataStruct weather_data_; //* weather infomation for use

            //*threading tuff 
        time_t last_retrived_;
        std::mutex mlock_;
        bool stop_thread = false;
        std::thread thread_;

        //*functions
        bool GetWeatherFromNatWeatherService();
        bool ParseXml();
        bool GetCurrentWeather();
            bool GetForecastWeather();
        bool PrintCurrentWeather();
            void RunThread();

    public:


        Weather() : thread_() {}
        -Weather() {
            stop_thread_ = true;
            if (thread_.joinable()) thread_.join();
        
        }

        bool Start() {
            thread_ = std::thread(&Weather::RunThread, this);
        
        }
};
#endif //WEATHER_WEATHER_H