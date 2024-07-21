package xxl.app.edit;

import pt.tecnico.uilib.menus.Command;
import pt.tecnico.uilib.menus.CommandException;
import xxl.Spreadsheet;
import xxl.exceptions.RangeException;

/**
 * Delete command.
 */
class DoDelete extends Command<Spreadsheet> {

    DoDelete(Spreadsheet receiver) {
        super(Label.DELETE, receiver);
	addStringField("range", Prompt.address());
    }

    @Override
    protected final void execute() throws CommandException {
	    try {
		    _receiver.delete(stringField("range"));
	    } catch (RangeException e) {
		    throw new InvalidCellRangeException(e.getMessage());
	    }
    }

}
