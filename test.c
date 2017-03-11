#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "matrix.h"

// test de la valeur lines apres appel a matrix_init
void test_init_lines(void){
	struct matrix *test = matrix_init(4,3);
	CU_ASSERT_EQUAL(test->lines,NULL);
	matrix_free(test);
	CU_PASS("Success matrix_init for lines");
}

// test de la valeur ncols apres appel a matrix_init
void test_init_ncols(void){
        struct matrix *test = matrix_init(4,3);
        CU_ASSERT_EQUAL(test->ncols,3);
        matrix_free(test);
        CU_PASS("Success matrix_init for ncols");
}

// test de la valeur nlines apres appel a matrix_init
void test_init_nlines(void){
        struct matrix *test = matrix_init(4,3);
        CU_ASSERT_EQUAL(test->nlines,4);
        matrix_free(test);
        CU_PASS("Success matrix_init for nlines");
}

//test de la methode matrix_get
void test_matrix_get(void){
	struct matrix *test = matrix_init(7,6);
	int try_1 = matrix_get(test,5,3);
	CU_ASSERT_EQUAL(try_1,0);

	matrix_set(test,4,2,33);
	int try_2 = matrix_get(test,4,2);
	CU_ASSERT_EQUAL(try_2,33);

	matrix_set(test,3,3,111);
	int try_3 = matrix_get(test,3,3);
	CU_ASSERT_EQUAL(try_3,111);

	matrix_free(test);

	CU_PASS("Success matrix_get");
}

//test de la methode matrix_set
void test_matrix_set(void){
	struct matrix *test = matrix_init(5,5);
	
	int try_1 = matrix_set(test,3,2,8);
	CU_ASSERT_EQUAL(try_1,0);
	int get_1 = matrix_get(test,3,2);
	CU_ASSERT_EQUAL(get_1,8);

	int try_2 = matrix_set(test,4,1,78);
	CU_ASSERT_EQUAL(try_2,0);
        int get_2 = matrix_get(test,4,1);
        CU_ASSERT_EQUAL(get_2,78);


	int try_3 = matrix_set(test,4,1,0);
	CU_ASSERT_EQUAL(try_3,0);
        int get_3 = matrix_get(test,4,1);
        CU_ASSERT_EQUAL(get_3,0);
   	
	int try_4 = matrix_set(test,4,4,33);
	CU_ASSERT_EQUAL(try_4,0);
        int get_4 = matrix_get(test,4,4);
        CU_ASSERT_EQUAL(get_4,33);

	int try_5 = matrix_set(test,0,0,11);
	CU_ASSERT_EQUAL(try_5,0);
        int get_5 = matrix_get(test,0,0);
        CU_ASSERT_EQUAL(get_5,11);

	matrix_free(test);

	CU_PASS("Success matrix_set");
}

//test de la methode matrix_add pour des matrices de meme dimension
void test_matrix_add_same_size(void){
	struct matrix *m1 = matrix_init(3,3);
        struct matrix *m2 = matrix_init(3,3);
	
        matrix_set(m1,0,0,4);
        matrix_set(m1,0,2,8);
        matrix_set(m1,1,0,7);
	matrix_set(m1,2,2,1);

    
        matrix_set(m2,0,0,8);
        matrix_set(m2,2,1,3);
        matrix_set(m2,1,1,4);
	matrix_set(m2,2,2,4);
	
	struct matrix *m3 = matrix_add(m1,m2);

	CU_ASSERT_EQUAL(matrix_get(m3,0,0),12);
	CU_ASSERT_EQUAL(matrix_get(m3,0,2),8);
	CU_ASSERT_EQUAL(matrix_get(m3,1,0),7);
	CU_ASSERT_EQUAL(matrix_get(m3,2,2),5);
	CU_ASSERT_EQUAL(matrix_get(m3,2,1),3);
	CU_ASSERT_EQUAL(matrix_get(m3,1,1),4);

        matrix_free(m1);
        matrix_free(m2);
        matrix_free(m3);

        CU_PASS("Success matrix_add_same_size");

}

//test de la methode matrix_add pour des matrices de dimension differente
void test_matrix_add_different_size(void){
	struct matrix *m1 = matrix_init(3,3);
        struct matrix *m2 = matrix_init(4,2);

	matrix_set(m1,0,0,4);
        matrix_set(m1,0,2,8);
        matrix_set(m1,1,0,7);
        matrix_set(m1,2,2,1);

    
        matrix_set(m2,0,0,8);
        matrix_set(m2,2,1,3);
        matrix_set(m2,1,1,4);
        matrix_set(m2,3,0,4);

        struct matrix *m3 = matrix_add(m1,m2);

	CU_ASSERT_EQUAL(matrix_get(m3,0,0),4);
        CU_ASSERT_EQUAL(matrix_get(m3,0,2),8);
	CU_ASSERT_EQUAL(matrix_get(m3,1,2),0);
        CU_ASSERT_EQUAL(matrix_get(m3,5,4),3);
        CU_ASSERT_EQUAL(matrix_get(m3,6,3),4);
	CU_ASSERT_EQUAL(matrix_get(m3,6,4),0);
        CU_ASSERT_EQUAL(matrix_get(m3,4,1),0);
        CU_ASSERT_EQUAL(matrix_get(m3,1,4),0);

        matrix_free(m1);
        matrix_free(m2);
        matrix_free(m3);

        CU_PASS("Success matrix_add_different_size");

}

//test de la methode matrix_transpose
void test_matrix_transpose(void){
	struct matrix *m1 = matrix_init(2,3);
	struct matrix *m2 = matrix_init(3,2);
    
   	matrix_set(m1,0,0,1);
    	matrix_set(m1,0,2,3);
    	matrix_set(m1,1,0,4);
    
   	matrix_set(m2,0,0,1);
    	matrix_set(m2,2,0,3);
    	matrix_set(m2,0,1,4);
    
    	struct matrix *m3 = matrix_transpose(m1);

	int i, j, k, l;
	for(i = 0; i < (m3->nlines); i++) {
        	for(j = 0; j < (m3->ncols); j++) {
            		k = matrix_get(m3,i,j);
            		l = matrix_get(m2,i,j);
            		CU_ASSERT_EQUAL(k,l);
        	}
    	}

	matrix_free(m1);
    	matrix_free(m2);
    	matrix_free(m3);

	CU_PASS("Success matrix_transpose");
}

//test de la methode matrix_convert
void test_matrix_convert(void){
	const int *array[3];
    
    	int arr_1[2] = { 1, 8 };
    	int arr_2[2] = { 9, 0 };
    	int arr_3[2] = { 5, 6 };
    
   	array[0] = arr_1;
    	array[1] = arr_2;
    	array[2] = arr_3;
	
	struct matrix *test = matrix_convert(array,3,2);

	CU_ASSERT_EQUAL(matrix_get(test,0,1),8);
	CU_ASSERT_EQUAL(matrix_get(test,1,1),0);
	CU_ASSERT_EQUAL(matrix_get(test,2,1),6);

	matrix_free(test);

	CU_PASS("Success matrix_convert");
}

//main
int main(int argc, char* argv[])
{
	CU_pSuite pSuite = NULL;

	if (CUE_SUCCESS != CU_initialize_registry())
	{
		return CU_get_error();
	}

	pSuite = CU_add_suite("test_bitwise_ops",NULL,NULL);
	if (pSuite == NULL)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ( (NULL == CU_add_test(pSuite, "test_init_lines", test_init_lines)) ||
		(NULL == CU_add_test(pSuite, "test_init_nlines", test_init_nlines)) ||
		(NULL == CU_add_test(pSuite, "test_init_ncols", test_init_ncols))  ||
		(NULL == CU_add_test(pSuite, "test_matrix_get", test_matrix_get))  ||
		(NULL == CU_add_test(pSuite, "test_matrix_set", test_matrix_set))  ||
		(NULL == CU_add_test(pSuite, "test_matrix_add_same_size", test_matrix_add_same_size)) ||
		(NULL == CU_add_test(pSuite, "test_matrix_add_different_size", test_matrix_add_different_size)) ||
		(NULL == CU_add_test(pSuite, "test_matrix_tranpose", test_matrix_transpose))  ||
		(NULL == CU_add_test(pSuite, "test_matrix_convert", test_matrix_convert)) 
		)
	{
      CU_cleanup_registry();
      return CU_get_error();
	}

    CU_basic_run_tests();
    CU_basic_show_failures(CU_get_failure_list());
    CU_cleanup_registry();
    return CU_get_error();
}

