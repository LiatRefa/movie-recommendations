
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"


void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
//  sp_movie movie = _recommender_system->get_movie(name, year);
  _movie_ranks.insert(std::pair<sp_movie, double>
          (_recommender_system->add_movie(name, year, features), rate));
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return _recommender_system->recommend_by_content(*this);
}

sp_movie  RSUser::get_recommendation_by_cf (int k) const
{
  return _recommender_system->recommend_by_cf(*this, k);
}

double RSUser::get_prediction_score_for_movie (const std::string &name,
                                               int year, int k) const
{
  sp_movie movie = _recommender_system->get_movie(name, year);
  if(!movie)
    {
      throw std::runtime_error("Error with movie.");
    }

  return _recommender_system->predict_movie_score(*this, movie, k);
}

std::ostream &operator<< (std::ostream &os, const RSUser &user)
{
  os << "name: " << user._name << std::endl;
  os<<*(user._recommender_system)<<std::endl;
  return os;
}
