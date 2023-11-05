package xxl.cells;

/**
 * Interface for an observer pattern. Classes that implement this
 * interface can be observers and can be notified of changes.
 */
public interface CellObserver {
    /** Method to notify the observer of an update/change. */
    void update();
}
