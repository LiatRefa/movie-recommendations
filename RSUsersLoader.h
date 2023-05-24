//
// Created on 2/21/2022.
//




#ifndef USERFACTORY_H
#define USERFACTORY_H
#include "RSUser.h"
#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class RSUsersLoader
{
private:
  static std::vector<sp_movie> line_movie (const std:: string &line,
                          std::shared_ptr<RecommenderSystem>& rs);
  static rank_map line_user(const std::vector<sp_movie> &movies, const
  std::string &line);

public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their
     * movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file(
        const std::string& users_file_path,
        std::unique_ptr<RecommenderSystem> rs) noexcept(false);

};


#endif //USERFACTORY_H
