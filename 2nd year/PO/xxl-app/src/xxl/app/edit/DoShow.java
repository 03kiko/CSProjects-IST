package xxl.app.edit;

import pt.tecnico.uilib.forms.Form;
import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import pt.tecnico.uilib.Dialog;
import xxl.Spreadsheet;
import xxl.exceptions.RangeException;
import xxl.exceptions.UnknownInputException;
import xxl.app.renderers.OutputRenderer;

/**
 * Class for searching functions.
 */
class DoShow extends Command<Spreadsheet> {

    DoShow(Spreadsheet receiver) {
        super(Label.SHOW, receiver);
	addStringField("range", Prompt.address());
    }

    @Override
    protected final void execute() throws CommandException {
        try {
		_display.popup(_receiver.view(stringField("range"), new OutputRenderer()));
        }
        catch (RangeException | UnknownInputException e) {
		throw new InvalidCellRangeException(e.getMessage());
        }
    }

}
