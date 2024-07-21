package xxl.cells.renderers;

/**
 * This interface is used to represent all
 * renderable objects.
 */
public interface Renderable {
	/**
	 * Accepts the renderer's visit.
	 *
	 * @param renderer how to render.
	 * @return the rendered object.
	 */
	String render(Renderer renderer);
}
