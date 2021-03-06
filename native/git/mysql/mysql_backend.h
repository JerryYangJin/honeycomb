#ifndef INCLUDE_git_mysql_backend_h__
#define INCLUDE_git_mysql_backend_h__

#include <assert.h>
#include <string.h>

#include "git2.h"

#ifdef GIT_WIN32
#include <winsock.h>
#endif

#include <mysql.h>

typedef struct {
  MYSQL *db;
  MYSQL_STMT *odb_read;
  MYSQL_RES  *meta_odb;
  MYSQL_STMT *odb_write;
  MYSQL_STMT *odb_read_header;
  MYSQL_STMT *index_read;
  MYSQL_STMT *index_write;
  MYSQL_STMT *index_del;
  MYSQL_STMT *refdb_read;
  MYSQL_RES  *meta_refdb;
  MYSQL_STMT *refdb_write;
  MYSQL_STMT *refdb_read_header;
  MYSQL_STMT *refdb_del;
  MYSQL_STMT *refdb_rename;
  MYSQL_STMT *repo_create;
  MYSQL_STMT *repo_del;
  MYSQL_STMT *repo_exists;
  MYSQL_STMT *tree_init;
  MYSQL_STMT *tree_update;
  MYSQL_STMT *tree_build;
  MYSQL_RES  *meta_tree_build;
  MYSQL_STMT *tree_root;
  MYSQL_RES  *meta_tree_root;
  MYSQL_STMT *config_get;
  MYSQL_RES  *meta_config;
  MYSQL_STMT *config_set;
  MYSQL_STMT *config_del;
} git_mysql;

int git_mysql_init(git_mysql **out,MYSQL *db);
int git_mysql_free(git_mysql *mysql);

/* Diagnostic function */
void print_bind(MYSQL_BIND *bind);

#endif
