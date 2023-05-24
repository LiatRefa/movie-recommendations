//
// Created on 2/20/2022.
//

#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
#include "RSUser.h"
#include "map"
#include "Movie.h"
#include <cmath>
#include "fstream"
#include "sstream"



class RecommenderSystem
{
 private:
  //definition of structures

  typedef std::map<sp_movie, std::vector<double>, compare_func> movies_data;
  typedef std::map<std::pair<double, sp_movie>, double, pair_func>
      best_movie_data;
  movies_data _recommender_sys_data;

  // private methods for recommendation algorithm
/***
 *
 * @param user current user
 * @return vector average calculated ranks
 */
  static std::vector<double> average_ranks(const RSUser &user);
  /***
   *
   * @param user curr user
   * @param recommender_sys_data the RecommandationSystem data
   * @param avg_ranks the vector with average calculated ranks
   * @return vector of features similarity
   */
  static std::vector<double> favourites_features(const RSUser &user,
                                                 movies_data
                                               &recommender_sys_data,
                                                 const std::vector<double>&
                                                   avg_ranks);
  /***
   * score a movie for the prediction algorithm
   * @param movie : the movie we check
   * @param predict : a movie to predict
   * @param recommender_sys_data the RecommandationSystem data
   * @return the predict score for the movie
   */
  static double movie_prediction_score(const sp_movie &movie,
                                   const sp_movie &predict,
                                   movies_data &recommender_sys_data);
  static sp_movie movie_best(const RSUser &user, std::vector<double> best,
                             movies_data &recommender_sys_data);


public:

	explicit RecommenderSystem();

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,int year,
                    const std::vector<double>& features);

    /**
     * a function that calculates the movie with highest score based on
     * movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score based
     * on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);


    /**
     * Predict a user rating for a movie given argument using item cf procedure
     * with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;


  friend std::ostream &
  operator<< (std::ostream &s, const RecommenderSystem &rs);


};


#endif //RECOMMENDERSYSTEM_H
