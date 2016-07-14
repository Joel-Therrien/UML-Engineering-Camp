// This #include statement was automatically added by the Particle IDE.
#include "spark-plotly.h"   //library with functions to access plotly (modified from what is available at build.particle.io so that it will work)
#include "SFE_BMP180.h"     //library taken from Sparkfun's code for accessing the BMP

#define NUM_TRACES 2

// You will need to create an SFE_BMP180 object, here called "pressure":

SFE_BMP180 pressure;

double baseline; // baseline pressure

char *streaming_tokens[NUM_TRACES] = {"<token #1>", "<token #2>"}; //Enter the tokens you generated under Plotly API settings

plotly graph = plotly("<your plotly username>", "<your API key>", streaming_tokens, "<name the plot>", NUM_TRACES); //Create a plotly graph with your username, API key, streaming tokens, and graph name

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Initialize the sensor (it is important to get calibration values stored on the device).

    if (pressure.begin())
    Serial.println("BMP180 init success");
    else
    {
      // Oops, something went wrong, this is usually a connection problem,
       // see the comments at the top of this sketch for the proper connections.

       Serial.println("BMP180 init fail (disconnected?)\n\n");
       while(1); // Pause forever.
    }
    graph.fileopt = "extend"; //This will keep adding data to the file instead of overwriting in case your Core ever needs to restart
    graph.convertTimestamp = "true"; //Convert millis to a date/time stamp
    graph.timezone = "America/New_York"; //Choose your timezone
    graph.init(); //Initialize the graph
    graph.openStream(); //Fire up the stream
    float baseline;
    baseline = getPressure();
}

void loop()
{
  float alt,T,P;
  int sec;
  bool wait=false;
  // Get a new pressure reading:
  sec = Time.second();
  if ( sec<=30 && !wait ){   //take a measurement once every minute
      P = getPressure();
      T = getTemperature();
      alt = pressure.altitude( P, baseline );
      Serial.print("Absolute pressure: ");
      Serial.print(P,1);
      Serial.print(" mbar, Altitude: ");
      if (T >= 0.0) Serial.print(" "); // add a space for positive numbers
      Serial.print(T,0);
      Serial.println(" meters");
      unsigned long x = millis(); //Variable to store the time in milliseconds that your Core has been alive
      graph.plot(x, alt, streaming_tokens[0]); //Add a timestamped altitude data point to the graph
      graph.plot(x, T, streaming_tokens[0]); //Add a timestamped temperature data point to the graph
      wait=true;    //make sure that the measurement is only done once since loop will likely run multimple times during each minute
    }
    else if( sec>30 && wait){     //OK, now that the minute is passed, reset the wait flag
        wait=false;
    }
}

double getPressure()
{
  char status;
  double T,P,p_temp=0;
  int n, count=0, samples=10;
  for(n=0;n<=samples;n++){
  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(p_temp,T);
        if (status != 0)
        {
          P+=p_temp;    //add current pressure measurement to running tally. Will be normalized back to average pressure at the end.
          count++;  //keep track of how many times the pressure was sucessfully measured
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
  }
  P = P/count;  //divide by number of measurements to get average
  return(P);
}

double getTemperature()
{
  char status;
  double T;
  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
  }
  return(T);
}
