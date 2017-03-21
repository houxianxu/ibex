import numpy as np

def PrecisionAndRecall(ground_truth, predictions):
    # make sure there are an equal number of elements
    assert (ground_truth.shape == predictions.shape)

    # make sure that the data is binary
    assert (np.amax(ground_truth) <= 1 and np.amax(predictions) <= 1)
    assert (np.amin(ground_truth) >= 0 and np.amax(predictions) >= 0)

    # set all of the counters to zero
    (TP, FP, FN, TN) = (0, 0, 0, 0)

    # iterate through every entry
    for ie in range(predictions.size):
        truth = ground_truth[ie]
        prediction = predictions[ie]

        if truth and prediction:
            TP += 1
        elif not truth and prediction:
            FP += 1
        elif truth and not prediction:
            FN += 1
        else:
            TN += 1

    print 'True Positives: ' + str(TP)
    print 'True Negatives: ' + str(TN)
    print 'False Positives: ' + str(FP)
    print 'False Negatives: ' + str(FN)

    print 'Precision: ' + str(float(TP) / float(TP + FP))
    print 'Recall: ' + str(float(TP) / float(TP + FN))
    print 'Accuracy: ' + str(float(TP + TN) / float(TP + FP + FN + TN))

    