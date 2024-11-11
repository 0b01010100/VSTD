
#define decl_array_index(val_t) \
	typedef val_t val_t##_array_index_t;

#define decl_array(val_t) \
	typedef struct val_t##len_t##_array_t val_t##len_t##_array_t;

#define decl_array_create(val_t) \
	val_t##len_t##_array_t val_t##len_t##_array_t_create();

#define decl_array_set(val_t) \
	void val_t##len_t##_array_t_set(val_t##len_t##_array_t * array, size_t index, val_t##_array_index_t);

#define decl_array_get(val_t) \
	val_t##_array_index_t val_t##len_t##_array_t_get(val_t##len_t##_array_t * array, size_t index);


#define decl_all(val_t)\
	decl_array_index(val_t)\
	decl_array(val_t)\
	decl_array_create(val_t)\
	decl_array_set(val_t)\
	decl_array_get(val_t)