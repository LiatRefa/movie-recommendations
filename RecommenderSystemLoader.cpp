#include "RecommenderSystemLoader.h"
#define MIN_SCORE 0
#define MAX_SCORE 10

std::unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  RecommenderSystem recommender_system;
  std::fstream f;
  f.open(movies_file_path);
  if(!f)
    {
      throw std::runtime_error ("Couldn't open file. check path: " +
      movies_file_path);
    }
    std::string line;
  while(std::getline(f, line))
    {
      parse_line(line, recommender_system);
    }
    f.close();
  return std::make_unique<RecommenderSystem>(recommender_system);

}

void RecommenderSystemLoader::parse_line (const std::string &line,
                                        RecommenderSystem &recommender_system)
{
  std::istringstream linestream(line);
  std::string name;
  int year;
  std::vector<double> features;
  std::getline(linestream, name, '-');
  linestream>> year;

 double num;
 while(linestream >> num)
   {
     if(num >= MIN_SCORE && num <= MAX_SCORE)
       {
         features.push_back(num);
       }
     else
       {
         throw std::invalid_argument("Feature value must be 0 <= x <= 10.");
       }
   }
   recommender_system.add_movie(name, year, features);
}