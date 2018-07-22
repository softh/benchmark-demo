package com.softhapps.benchamrk;

import java.util.List;

/**
 * It is the result of the task of finding primes
 *
 * @author softh
 */
public class ExecutionResult {

    /**
     * Execution time in milliseconds
     */
    private long totalTime;

    /**
     * Execution result
     */
    private List<Long> resultPrimes;

    public ExecutionResult(long totalTime, List<Long> resultPrimes) {
        this.totalTime = totalTime;
        this.resultPrimes = resultPrimes;
    }

    public long getTotalTime() {
        return totalTime;
    }

    public List<Long> getResultPrimes() {
        return resultPrimes;
    }
}
