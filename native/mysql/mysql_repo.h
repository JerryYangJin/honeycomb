#ifndef INCLUDE_git_mysql_repo_h__
#define INCLUDE_git_mysql_repo_h__

#include "git2.h"
#include "mysql_backend.h"

GIT_BEGIN_DECL

GIT_EXTERN(long long int) git_mysql_last_seq(git_mysql *mysql);

GIT_EXTERN(long long int) git_mysql_repo_create(git_mysql *mysql, const long long int owner, const char *name, const char *description);

GIT_EXTERN(int) git_mysql_repo_del(git_mysql *mysql);

GIT_END_DECL

#endif
