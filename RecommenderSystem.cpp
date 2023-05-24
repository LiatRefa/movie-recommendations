
#include "RecommenderSystem.h"
#include <cmath>
#include <map>
#include <limits>
#include <cfloat>
#define IMAGINE_NORM(NUM1, NUM2, NUM3) (NUM1 / (sqrt (NUM2) * sqrt (NUM3)))



RecommenderSystem::RecommenderSystem () :
_recommender_sys_data(sp_movie_compare)
{}


sp_movie RecommenderSystem::add_movie (const std::string &name,
                                       int year, const std::vector<double>
                                       &features)
{
  sp_movie movie = std::make_shared<Movie>(Movie(name, year));
  _recommender_sys_data.insert(std::pair<sp_movie, std::vector<double>>
                                   (movie, features));
  return movie;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  //stage 1: get average to decrease it to normalize
  std::vector<double> average_vec = average_ranks(user);
  std::vector<double> favourites;
  // stage 2: calculate favourite features
  favourites.resize (user.get_ranks().size());
  favourites = favourites_features(user, _recommender_sys_data,
                                   average_vec);
  // stage 3: calc imagination vec and get the most similiar one in features
  return movie_best(user, favourites, _recommender_sys_data);
}

sp_movie RecommenderSystem::recommend_by_cf (const RSUser &user, int k)
{
  sp_movie recommended = nullptr;
  double max_pred = 0;
  double curr_score = 0;
  for(const auto &unwatched: _recommender_sys_data)
    {
      if(user.get_ranks().find(unwatched.first) == user.get_ranks().end())
        {
          curr_score = predict_movie_score(user, unwatched.first, k);
          if(curr_score > max_pred)
            {
              max_pred = curr_score;
              recommended = unwatched.first;
            }
        }
    }
    return recommended;
}

double RecommenderSystem::predict_movie_score (const RSUser &user,
                                               const sp_movie &movie, int k)
{
  best_movie_data best_match (sp_movie_pair_ranks);
  double score;

  for(const auto& unwatched : user.get_ranks())
    {
      if (unwatched.second != 0)
        {
          score = movie_prediction_score(unwatched.first, movie,
                                         _recommender_sys_data);
          best_match[std::make_pair(score, unwatched.first)] = unwatched
              .second;
        }
    }
  double max = 0;
  double min = 0;
  for(auto mov = best_match.rbegin(); mov != best_match.rend(); mov++)
    {
      if(k != 0)
        {
          k--;
          min += mov->first.first;
          max += mov->first.first * user.get_ranks()[mov->first.second];
        }
      else{
        break;
      }
    }
    return max / min;
}


sp_movie RecommenderSystem::get_movie (const std::string &name, int year) const
{
  sp_movie movie_ptr = std::make_shared<Movie>(Movie(name, year));
  auto entry = this->_recommender_sys_data.find(movie_ptr);
  if(entry != this->_recommender_sys_data.end())
    {
      return entry->first;
    }
  return {nullptr};
}

std::ostream& operator<< (std::ostream &os,
    const RecommenderSystem& recommender_system)
{
  for(const auto& item: recommender_system._recommender_sys_data)
    {
      os << *(item.first);
    }
  return os;
}

// private methods:

std::vector<double> RecommenderSystem::average_ranks (const RSUser &user)
{
  int i = 0;
  double sum = 0;
  int n = 0;
  std::vector<double> minus_vec;
  minus_vec.resize(user.get_ranks().size());
  for(const auto& rank: user.get_ranks())
    {
      sum += rank.second;
      n++;
    }
    sum = sum / n;
  for (const auto& rank:user.get_ranks())
    {
      minus_vec[i] = rank.second - sum;
      i++;
    }
  return minus_vec;
}

std::vector<double> RecommenderSystem::favourites_features(const RSUser &user,
                                           movies_data &recommender_sys_data,
                                           const std::vector<double> &avg_ranks)
{
  std::vector<double> func;
  std::vector<double> tmp;
  func.resize(recommender_sys_data.begin()->second.size());
  int i=0;
  for(const auto& curr_movie: user.get_ranks())
    {
     for(long unsigned int j=0;
     j < recommender_sys_data.begin()->second.size(); j++)
       {
         func[j] += avg_ranks[i] * recommender_sys_data[curr_movie.first][j];
       }
       i++;
    }
    return func;
}


sp_movie RecommenderSystem::movie_best (const RSUser &user,
                                        std::vector<double> best,
                                        movies_data &recommender_sys_data)
{
  double st_norm = 0, nd_norm = 0, rd_norm = 0;
  double max_match = -DBL_MAX;
  sp_movie closest_score = nullptr;

  for(const auto &movie_obj: recommender_sys_data)
    {
      if(user.get_ranks().find(movie_obj.first) == user.get_ranks().end())
      {
       auto others_freq = recommender_sys_data[movie_obj.first];
       for(long unsigned int i = 0; i<best.size(); ++i)
         {
           st_norm += best[i] * others_freq[i];
           nd_norm += best[i] * best[i];
           rd_norm += others_freq[i] * others_freq[i];
         }
         if (nd_norm == 0)
           {
             return movie_obj.first;
           }
         else{
           if(IMAGINE_NORM(st_norm, nd_norm, rd_norm) > max_match)
             {
               max_match = IMAGINE_NORM(st_norm, nd_norm, rd_norm);
               closest_score = movie_obj.first;
             }
         }
      }
    }
  return closest_score;
}

double RecommenderSystem::movie_prediction_score (const sp_movie &movie,
                                                  const sp_movie &predict,
                                          movies_data &recommender_sys_data)
{
  double st_norm = 0, nd_norm = 0, rd_norm = 0;
  for(long unsigned int i=0; i<recommender_sys_data[movie].size(); i++)
    {
      st_norm += recommender_sys_data[movie][i] *
          recommender_sys_data[predict][i];
      nd_norm += recommender_sys_data[movie][i]
          *recommender_sys_data[movie][i];
      rd_norm += recommender_sys_data[predict][i] *
          recommender_sys_data[predict][i];
    }
    return IMAGINE_NORM(st_norm,nd_norm,rd_norm);
}
