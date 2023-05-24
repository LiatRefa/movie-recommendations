
#ifndef INC_23A_C_C__EX5_MOVIE_H
#define INC_23A_C_C__EX5_MOVIE_H

#include <iostream>
#include <vector>
#include <memory>

#define HASH_START 17



class Movie;

typedef std::shared_ptr<Movie> sp_movie; // define your smart pointer

/**
 * those declartions and typedefs are given to you and should be used in the ex
 */
typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);
std::size_t sp_movie_hash(const sp_movie& movie);
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

typedef bool (*compare_func) (const sp_movie &movie, const sp_movie &other);
typedef bool (*pair_func) (const std::pair<double, sp_movie>& pair,
                           const std::pair<double, sp_movie>& another);
bool sp_movie_compare(const sp_movie& movie,const sp_movie& other);
bool sp_movie_pair_ranks(const std::pair<double, sp_movie>& pair,
                         const std::pair<double, sp_movie>& another);

class Movie
{
 private:
  const std::string _name;
  int _year;
   
public:
    /**
     * constructor
     * @param name: name of movie
     * @param year: year it was made
     */
    Movie(const std::string& name, int year): _name(name), _year(year) {};

    /**
     * returns the name of the movie
     * @return const ref to name of movie
     */
     const std::string& get_name() const
     {
       return _name;
     }


    /**
     * returns the year the movie was made
     * @return year movie was made
     */
    int get_year() const
    {
      return _year;
    }

	/**
     * operator< for two movies
     * @param rhs: right hand side
     * @param lhs: left hand side
     * @return returns true if (lhs.year) < rhs.year or
     * (rhs.year == lhs.year & lhs.name < rhs.name) else return false
     */

    bool operator< (const Movie& other) const;

    /**
     * operator<< for movie
     * @param os ostream to output info with
     * @param movie movie to output
     */
    friend std::ostream& operator<< (std::ostream& os, const Movie& movie);
};


#endif //INC_23A_C_C__EX5_MOVIE_H
