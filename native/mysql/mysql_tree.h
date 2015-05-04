#ifndef INCLUDE_git_mysql_tree_h__
#define INCLUDE_git_mysql_tree_h__

#include "git2.h"
#include "mysql_backend.h"

GIT_BEGIN_DECL

GIT_EXTERN(int) git_mysql_tree_init(git_mysql *mysql);

GIT_EXTERN(int) git_mysql_tree_build(git_mysql *mysql, git_repository *repo, const char *type);

GIT_EXTERN(git_tree *) git_mysql_tree_root(git_mysql *mysql, git_repository *repo);

GIT_EXTERN(int) git_mysql_tree_walk(git_mysql *mysql, git_repository *repo);

GIT_EXTERN(int) git_mysql_tree_diff(git_mysql *mysql, git_repository *repo, git_diff *diff);

GIT_END_DECL

#endif