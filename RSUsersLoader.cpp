#define MIN_SCORE 0
#define MAX_SCORE 10
#define EMPTY "NA"
#include "RSUsersLoader.h"

std::vector<RSUser> RSUsersLoader::create_users_from_file(
    const std::string &users_file_path,
    std::unique_ptr<RecommenderSystem> rs) noexcept (false)
{
  std::ifstream f;
  f.open(users_file_path);
  if(!f)
    {
      throw std::runtime_error ("Couldn't open file.");
    }
  std::shared_ptr<RecommenderSystem> new_rs = std::move(rs);
  std::vector<RSUser> users;
  std::string line;
  std::string data;
  getline(f, line);
  std::vector<sp_movie> movie_vec = line_movie(line, new_rs);
  rank_map user_map (0, sp_movie_hash, sp_movie_equal);

  while(getline(f, line))
    {
      std::stringstream line_stream (line);
      line_stream >> data;
      user_map = line_user(movie_vec, line);
      users.emplace_back(RSUser(data, user_map, new_rs));
    }
  return users;
}

std::vector<sp_movie> RSUsersLoader::line_movie (const std::string &line,
                                     std::shared_ptr<RecommenderSystem> &rs)
{
  std::vector<sp_movie> input_movies;
  std::stringstream line_stream(line);
  std::string name;
  int year;
  std::string double_words;

  while(line_stream >> double_words)
    {
      std::stringstream words (double_words);
      std::getline(words, name, '-');
      words >> year;
      input_movies.emplace_back(rs->get_movie(name, year));
    }
    return input_movies;
}

rank_map RSUsersLoader::line_user (const std::vector<sp_movie> &movies,
                                   const std::string &line)
{
  std::string name, rate;
  std::stringstream linestream (line);
  linestream >> name;
  rank_map user_map (0, sp_movie_hash, sp_movie_equal);
  for( const auto &movie : movies)
    {
      linestream >> rate;
      if(!(rate.compare(EMPTY)))
      {
          continue;
      }
      else
        {
          double rank = std::stod(rate);
          if(rank >= MIN_SCORE && rank <= MAX_SCORE)
            {
              user_map.insert(std::pair<sp_movie, double> (movie, rank));
            }
          else
          {
            throw  std::invalid_argument("Score must be in 0<=x<=10.");
          }
        }
    }
    return user_map;
}

