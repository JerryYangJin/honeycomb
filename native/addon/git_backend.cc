#include <nan.h>
#include <string.h>
#include <chrono>
#include <thread>

#include "git_backend.h"

void GitBackend::InitializeComponent(Handle<v8::Object> target) {
    NanScope();

    Local<Object> object = NanNew<Object>();

	NODE_SET_METHOD(object, "gitMysqlOpen", GitMysqlOpen);

	NODE_SET_METHOD(object, "gitMysqlClose", GitMysqlClose);

	NODE_SET_METHOD(object, "gitMysqlCreateBlob", GitMysqlCreateBlob);

	NODE_SET_METHOD(object, "gitMysqlWriteTree", GitMysqlWriteTree);

	NODE_SET_METHOD(object, "gitMysqlLookup", GitMysqlLookup);

	NODE_SET_METHOD(object, "gitMysqlCreateRef", GitMysqlCreateRef);

	NODE_SET_METHOD(object, "gitMysqlCommit", GitMysqlCommit);

    target->Set(NanNew<String>("SQL"), object);
}


NAN_METHOD(GitBackend::GitMysqlOpen) {
  NanEscapableScope();

  if (args.Length() == 0 || !args[0]->IsString()) {
    return NanThrowError("mysql host is required.");
  }

  if (args.Length() == 1 || !args[1]->IsString()) {
    return NanThrowError("mysql user is required.");
  }

  if (args.Length() == 2 || !args[2]->IsString()) {
    return NanThrowError("mysql password is required.");
  }

  if (args.Length() == 3 || !args[3]->IsString()) {
    return NanThrowError("mysql db is required.");
  }

  if (args.Length() == 4 || !args[4]->IsNumber()) {
    return NanThrowError("mysql port required.");
  }

// start convert_from_v8 block
  const char * from_mysql_host;
  String::Utf8Value mysql_host(args[0]->ToString());
  from_mysql_host = (const char *) strdup(*mysql_host);

  const char * from_mysql_user;
  String::Utf8Value mysql_user(args[1]->ToString());
  from_mysql_user = (const char *) strdup(*mysql_user);

  const char * from_mysql_passwd;
  String::Utf8Value mysql_passwd(args[2]->ToString());
  from_mysql_passwd = (const char *) strdup(*mysql_passwd);

  const char * from_mysql_db;
  String::Utf8Value mysql_db(args[3]->ToString());
  from_mysql_db = (const char *) strdup(*mysql_db);

  unsigned int from_mysql_port;
  from_mysql_port = (unsigned int)args[4]->ToNumber()->Value();
// end convert_from_v8 block

  int error = git_mysql_init(&mysql,
							  from_mysql_host,
							  from_mysql_user,
							  from_mysql_passwd,
							  from_mysql_db,
							  from_mysql_port,
							  NULL,
							  0);

  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_mysql_init error")));
	  return Undefined();
  }

  error = git_libgit2_init();
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_libgit2_init error")));
	  return Undefined();
  }

  git_odb_backend   *odb_backend;
  error = git_mysql_odb_init(&odb_backend, mysql);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_mysql_odb_init error")));
	  return Undefined();
  }

  error = git_odb_new(&odb_backend->odb);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_odb_new error")));
	  return Undefined();
  }

  error = git_odb_add_backend(odb_backend->odb, odb_backend, 1);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_odb_add_backend error")));
	  return Undefined();
  }

  error = git_repository_wrap_odb(&repo, odb_backend->odb);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_repository_wrap_odb error")));
	  return Undefined();
  }
  
  git_refdb *refdb;
  error = git_refdb_new(&refdb,repo);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_refdb_new error")));
	  return Undefined();
  }

  git_refdb_backend   *refdb_backend;
  error = git_mysql_refdb_init(&refdb_backend, mysql);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_mysql_refdb_init error")));
	  return Undefined();
  }

  error = git_refdb_set_backend(refdb, refdb_backend);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_mysql_refdb_init error")));
	  return Undefined();
  }

  git_repository_set_refdb(repo, refdb);

  git_config *config;
  error = git_config_open_default(&config);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_config_open_default error")));
	  return Undefined();
  }

  git_repository_set_config(repo,config);

  
  error = git_repository_set_workdir(repo, "/", 0);
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_repository_set_workdir error")));
	  return Undefined();
  }
   //printf("workdir:%s", git_repository_workdir(repo));
  
  
  
  error = git_repository_set_path(repo, "/");
  if (error < 0){
	  ThrowException(Exception::TypeError(String::New("git_repository_set_path error")));
	  return Undefined();
  } 
  //printf("path:%s", git_repository_path(repo));
  
  //Handle<v8::Value> obj = GitMysql::New(from_out, false);
  //NanReturnValue(obj);
  if (!error)
	  NanReturnValue(NanTrue());
  else
	  NanReturnValue(NanFalse());

}

NAN_METHOD(GitBackend::GitMysqlClose) {
	NanEscapableScope();

	int error = git_mysql_free(mysql);

	error = git_libgit2_shutdown();
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_libgit2_shutdown error")));
		return Undefined();
	}

	if (!error)
		NanReturnValue(NanTrue());
	else
		NanReturnValue(NanFalse());
}

NAN_METHOD(GitBackend::GitMysqlCreateBlob) {
	NanEscapableScope();

	if (args.Length() == 0 || !args[0]->IsString()) {
		return NanThrowError("path is required.");
	}

	if (args.Length() == 1 || !args[1]->IsString()) {
		return NanThrowError("buffer is required.");
	}

	// start convert_from_v8 block
	const char *from_path;
	String::Utf8Value path_buf(args[0]->ToString());
	from_path = (const char *)strdup(*path_buf);

	const char *from_buf;
	String::Utf8Value blob_buf(args[1]->ToString());
	from_buf = (const char *)strdup(*blob_buf);
	// end convert_from_v8 block

	int				  error;
	git_oid			  oid;

	// Transaction Start
	git_mysql_transaction(mysql);

	error = git_blob_create_frombuffer(&oid, repo, from_buf, strlen(from_buf));
	if (error < 0){
		git_mysql_rollback(mysql);
		ThrowException(Exception::TypeError(String::New("git_blob_create_frombuffer error")));
		return Undefined();
	}

	error = git_mysql_index_write(mysql, &oid, from_path);
	if (error < 0){
		git_mysql_rollback(mysql);
		ThrowException(Exception::TypeError(String::New("git_mysql_index_write error")));
		return Undefined();
	}

	git_mysql_commit(mysql);

	char sha1[GIT_OID_HEXSZ+1] = { 0 };
	git_oid_tostr(sha1, GIT_OID_HEXSZ+1, &oid);

	Handle<v8::Value> to;
	to = NanNew<String>(sha1);

	NanReturnValue(to);
}

NAN_METHOD(GitBackend::GitMysqlWriteTree) {
	NanEscapableScope();

	int				  error;
	git_oid			  oid,tree;
	git_treebuilder   *bld = NULL;

	git_oid_fromstr(&oid, "41842f42ac3734774a6192c9d7f03ad972c684fc");

	error = git_treebuilder_new(&bld, repo, NULL);
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_treebuilder_new error")));
		return Undefined();
	}

	error = git_treebuilder_insert(NULL, bld, "README.md", &oid, GIT_FILEMODE_BLOB);
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_treebuilder_insert error")));
		return Undefined();
	}

	// Transaction Start
	git_mysql_transaction(mysql);

	error = git_treebuilder_write(&tree, bld);
	if (error < 0){
		git_mysql_rollback(mysql);
		ThrowException(Exception::TypeError(String::New("git_treebuilder_write error")));
		return Undefined();
	}

	git_mysql_commit(mysql);

	git_treebuilder_free(bld);

	char sha1[GIT_OID_HEXSZ + 2] = { 0 };
	git_oid_tostr(sha1, GIT_OID_HEXSZ + 1, &tree);

	Handle<v8::Value> to;
	to = NanNew<String>(sha1);

	NanReturnValue(to);
}

NAN_METHOD(GitBackend::GitMysqlLookup) {
	NanEscapableScope();

	int				  error;
	git_oid			  oid;
	git_object *	  obj;

	if (args.Length() == 0 || !args[0]->IsString()) {
		return NanThrowError("path is required.");
	}

	// start convert_from_v8 block
	const char *from_sha1;
	String::Utf8Value sha1(args[0]->ToString());
	from_sha1 = (const char *)strdup(*sha1);
	// end convert_from_v8 block

	git_oid_fromstr(&oid, from_sha1);

	error = git_object_lookup(&obj, repo, &oid, GIT_OBJ_ANY);
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_object_lookup error")));
		return Undefined();
	}

	Handle<v8::Value> to;


	git_blob *blob = (git_blob*)obj;
	git_off_t rawsize = git_blob_rawsize(blob);
	const char *rawcontent = (char *)git_blob_rawcontent(blob);
	
	to = NanNew<String>(rawcontent);
	
	NanReturnValue(to);
}

NAN_METHOD(GitBackend::GitMysqlCreateRef) {
	NanEscapableScope();

	if (args.Length() == 0 || !args[0]->IsString()) {
		return NanThrowError("name is required.");
	}

	if (args.Length() == 1 || !args[1]->IsString()) {
		return NanThrowError("target is required.");
	}

	// start convert_from_v8 block
	const char *from_name;
	String::Utf8Value name_buf(args[0]->ToString());
	from_name = (const char *)strdup(*name_buf);

	const char *from_target;
	String::Utf8Value target_buf(args[1]->ToString());
	from_target = (const char *)strdup(*target_buf);
	// end convert_from_v8 block

	int				  error;
	git_oid			  oid;
	git_reference *ref = NULL;

	git_oid_fromstr(&oid, from_target);

	// Transaction Start
	git_mysql_transaction(mysql);

	error = git_reference_create(&ref, repo, from_name, &oid, true, NULL, NULL);
	if (error < 0){
		git_mysql_rollback(mysql);
		ThrowException(Exception::TypeError(String::New("git_reference_create error")));
		return Undefined();
	}

	git_mysql_commit(mysql);

	if (!error)
		NanReturnValue(NanTrue());
	else
		NanReturnValue(NanFalse());
}

NAN_METHOD(GitBackend::GitMysqlCommit) {
	NanEscapableScope();

	if (args.Length() == 0 || !args[0]->IsString()) {
		return NanThrowError("ref is required.");
	}

	if (args.Length() == 1 || !args[1]->IsString()) {
		return NanThrowError("message is required.");
	}

	// start convert_from_v8 block
	const char *from_ref;
	String::Utf8Value ref_buf(args[0]->ToString());
	from_ref = (const char *)strdup(*ref_buf);

	const char *from_msg;
	String::Utf8Value msg_buf(args[1]->ToString());
	from_msg = (const char *)strdup(*msg_buf);
	// end convert_from_v8 block

	int				  error;
	git_oid oid;
	git_signature *me;
	git_oid commit;
	git_tree *tree;

	//error = git_revparse_single((git_object**)&tree, repo, "HEAD~^{tree}");
	//if (error < 0){
	//	git_mysql_rollback(mysql);
	//	ThrowException(Exception::TypeError(String::New("git_revparse_single error")));
	//	return Undefined();
	//}
	git_oid_fromstr(&oid, "e1ca16979da4db87b96af5268ac2ba8facb1a4f4");
	

	error = git_tree_lookup(&tree, repo, &oid);
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_tree_lookup error")));
		return Undefined();
	}

	error = git_signature_now(&me, "Me", "me@example.com");
	if (error < 0){
		ThrowException(Exception::TypeError(String::New("git_signature_now error")));
		return Undefined();
	}

	// Transaction Start
	git_mysql_transaction(mysql);

	error = git_commit_create(&commit, repo, from_ref, me, me, "UTF-8", from_msg, tree, 0, NULL);
	if (error < 0){
		git_mysql_rollback(mysql);
		ThrowException(Exception::TypeError(String::New("git_commit_create error")));
		return Undefined();
	}

	git_mysql_commit(mysql);

	char sha1[GIT_OID_HEXSZ + 1] = { 0 };
	git_oid_tostr(sha1, GIT_OID_HEXSZ + 1, &commit);

	Handle<v8::Value> to;
	to = NanNew<String>(sha1);

	NanReturnValue(to);
}

Persistent<Function> GitBackend::constructor_template;
git_mysql * GitBackend::mysql;
git_repository *GitBackend::repo;