//
// Created by softh on 21.07.2018.
//
#include <jni.h>
#include <math.h>
#include <cstdlib>
#include <vector>

/**
 * Returns simple digest with zero empty values (Sieve of Atkin)
 */
extern "C"
JNIEXPORT
jlongArray
JNICALL
Java_com_softhapps_benchamrk_MainActivity_getSimplesAtkin(JNIEnv *env, jobject, jlong end_position) {
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
#else
#define ABI "armeabi-v7a"
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#endif

    long size = end_position;

    jlong buffer_size = size + 1;
    bool *is_simple = (bool *) malloc(buffer_size);

    long sqr_lim;
    long x2, y2;
    long i, j;
    long n;

    /**
     * Initialize
     */
    sqr_lim = (long) sqrt(buffer_size);
    for (i = 0; i <= buffer_size; i++) {
        is_simple[i] = false;
    }
    is_simple[2] = true;
    is_simple[3] = true;

    x2 = 0;
    for (i = 1; i <= sqr_lim; i++) {
        x2 += 2 * i - 1;
        y2 = 0;
        for (j = 1; j <= sqr_lim; j++) {
            y2 += 2 * j - 1;

            n = 4 * x2 + y2;
            if ((n <= buffer_size) && (n % 12 == 1 || n % 12 == 5))
                is_simple[n] = !is_simple[n];

            // n = 3 * x2 + y2;
            n -= x2;
            if ((n <= buffer_size) && (n % 12 == 7))
                is_simple[n] = !is_simple[n];

            // n = 3 * x2 - y2;
            n -= 2 * y2;
            if ((i > j) && (n <= buffer_size) && (n % 12 == 11))
                is_simple[n] = !is_simple[n];
        }
    }

    for (i = 5; i <= sqr_lim; i++) {
        if (is_simple[i]) {
            n = i * i;
            for (j = n; j <= buffer_size; j += n) {
                is_simple[j] = false;
            }
        }
    }

    int simple_size = 0;

    for (int i = 0; i < buffer_size; i++) {
        if (is_simple[i]) {
            simple_size++;
        }
    }

    jlongArray result = env->NewLongArray(simple_size);

    jlong *result_reference = env->GetLongArrayElements(result, 0);

    for (long i = 0, k = 0; i < buffer_size; i++) {
        if (is_simple[i]) {
            result_reference[k++] = i;
        }
    }

    env->SetLongArrayRegion(result, 0, simple_size, result_reference);

    return result;
}

extern "C"
JNIEXPORT
jlongArray
JNICALL
Java_com_softhapps_benchamrk_MainActivity_getSimplesAks(JNIEnv *env, jobject, jlong start_position, jlong end_position) {

//#define size(a) (sizeof(a) / sizeof(*a))

    int buffer_size_increment = 80;
    int buffer_size = buffer_size_increment;
    int simples_counter = 0;
    int buffer_counter = buffer_size / sizeof(long);

    int coefficients_count = (int) ((end_position - start_position) * 2);

    jlong *simples = (jlong *) malloc(buffer_size);

    long *coefficients = (long *) malloc(coefficients_count);

    for (long number = start_position; number < end_position; number++) {

        for (int j = 0; j < coefficients_count; j++) {
            coefficients[j] = 0;
        }

        coefficients[0] = 1;
        for (int i = 0; i < number; coefficients[0] = -coefficients[0], i++) {
            coefficients[1 + i] = 1;

            for (int j = i; j > 0; j--)
                coefficients[j] = coefficients[j - 1] - coefficients[j];
        }

        coefficients[0]++;
        coefficients[number]--;

        long i = number;
        while (i-- && coefficients[i] % number == 0);

        if (i < 0) {
            if (buffer_counter <= simples_counter) {
                buffer_counter += (buffer_size_increment / sizeof(long));
                simples = (jlong *) realloc(simples, buffer_counter * sizeof(jlong));
            }

            simples[simples_counter++] = number;
        }
    }

    free(coefficients);
    jlongArray result = env->NewLongArray(simples_counter);
    env->SetLongArrayRegion(result, 0, simples_counter, simples);
    return result;
}

extern "C"
JNIEXPORT
jlongArray
JNICALL
Java_com_softhapps_benchamrk_MainActivity_getSimplesMillerRabin(JNIEnv *env, jobject, jlong start_position,
                                                                jlong end_position) {

    int primes_counter = 0;
    int buffer_size_increment = 80;
    int buffer_size = 80;

    jlong *primes = (jlong *) malloc(buffer_size);

    long as[] = {2, 7, 61};

    for (long number = start_position; number <= end_position; number++) {

        bool is_prime = false;

        for (int j = 0; j < 3; j++) {
            outer_mark:
            long a = as[j];
            if (a < number) {
                long s = 0;
                long d = number - 1;
                while (d % 2 == 0) {
                    s++;
                    d /= 2;
                }

                //modpow
                long modpow_result = 1;
                long aktpot = a;
                long c = d;
                while (c > 0) {
                    if (c % 2 == 1) {
                        modpow_result = (modpow_result * aktpot) % number;
                    }
                    aktpot = (aktpot * aktpot) % number;
                    c /= 2;
                }

                long x = modpow_result;

                if (x != 1 && x != number - 1) {
                    for (long r = 1; r < s; r++) {
                        x = (x * x) % number;
                        if (x == 1) {
                            is_prime = false;
                            goto result_mark;
                        }
                        if (x == number - 1) {
                            j++;
                            goto outer_mark;
                        }
                    }
                    is_prime = false;
                    goto result_mark;
                }
            }
        }

        is_prime = true;

        result_mark:
        if (is_prime) {
            if (buffer_size <= primes_counter) {
                buffer_size += (buffer_size_increment / sizeof(long));
                primes = (jlong *) realloc(primes, buffer_size * sizeof(jlong));
            }

            primes[primes_counter++] = number;
        }
    }

    for(int i = 0; i < primes_counter;i++) {
        if(primes[i] == 0) {
            int sssaqa = 0;
        }
    }

    jlongArray result = env->NewLongArray(primes_counter);
    env->SetLongArrayRegion(result, 0, primes_counter, primes);
    return result;

}